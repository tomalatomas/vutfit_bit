//
//  CategoriesVC.swift
//  Budget
//


import UIKit

class CategoriesVC: UITableViewController {
    
    @IBOutlet weak var addCategoryButton: UIBarButtonItem!
    
    let context = (UIApplication.shared.delegate as! AppDelegate).persistentContainer.viewContext
    

    private var categories  = [Category]()
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.tableFooterView = UIView()
        getAllCategories()
        self.navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .add, target: self, action: #selector(didTapAdd))
    }
    override func viewDidAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        getAllCategories()
        DispatchQueue.main.async {
            self.tableView.reloadData()
        }
    }
    
    /// Gets all categories and stores it into a variable "catogories"
    ///
    func getAllCategories() {
        do {
            categories = try context.fetch(Category.fetchRequest())
            DispatchQueue.main.async {
                self.tableView.reloadData()
            }
        } catch {
            abort()
        }
    }

    // MARK: - Table view data source

    /// Sets number of section in table view
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Returns: number of sections
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }

    /// Sets number of rows in section
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: section to configure
    /// - Returns: number of row in section
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return categories.count
    }
    
    /// Sets table view cell into tableview
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter indexPath: for which row andsection to configure cell
    /// - Returns: table view cell
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let category = categories[indexPath.row]
        let cell = tableView.dequeueReusableCell(withIdentifier: "CategoryCell", for: indexPath) as! CategoryVCell
        cell.setName(name: category.name)
        return cell
    }
    
    /// Shows possible actions when swiping left on table cell
    ///
    /// - Parameter tableView: in which tableView
    /// - Parameter indexPath: for which row and section to configure actions
    /// - Returns: possible actions
    override func tableView(_ tableView: UITableView, trailingSwipeActionsConfigurationForRowAt indexPath: IndexPath) -> UISwipeActionsConfiguration? {
        let edit = editCategoryAction(at: indexPath)
        if indexPath.row == 0 {
            return UISwipeActionsConfiguration(actions: [edit])
        }
        let delete = deleteCategory(at: indexPath)
        return UISwipeActionsConfiguration(actions: [delete,edit])
    }
    
    /// Row was selected
    ///
    /// - Parameter tableView: in which tableview
    /// - Parameter indexPath: which row in which section
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let category = categories[indexPath.row]
        let _sb = UIStoryboard(name:"Main", bundle: nil)
        guard let _vc = _sb.instantiateViewController(identifier: "CategoryTransactions") as? TransactionsVC else {abort()}
        //_vc.transactions = _vc.getTransactions(of: category)
        _vc.category = category
        self.navigationController?.pushViewController(_vc, animated: true)
    }
    

    // MARK: - Creating category
    
    /// Shows popup window for creating new category
    ///
    @IBAction func didTapAdd() {
        let alert = UIAlertController(title: "New Category", message: "Enter new category", preferredStyle: .alert)
        alert.addTextField(configurationHandler: nil)
        alert.addAction(UIAlertAction(title: "Create", style: .default ,handler: {[weak self] _ in
            guard let field = alert.textFields?.first, let text = field.text, !text.isEmpty else {
                return
            }
            self?.createCategory(name: text)
        }))
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel,handler: nil))
        present(alert, animated: true)
    }
    /// Creates new category and saves it into user data
    ///
    /// - Parameter name: name of category
    func createCategory(name:String){
        let newCategory = Category(context:context)
        newCategory.name = name
        do{
            try context.save()
            getAllCategories()
        } catch{
            abort()
        }
    }
    // MARK: - Deleting category
    
    /// Deletes category from user data
    ///
    /// - Parameter indexPath: which row
    func deleteCategory(at indexPath:IndexPath) -> UIContextualAction {
        let category = categories[indexPath.row]
        let action = UIContextualAction(style: .destructive, title: "Delete"){
            (action,view,completion) in completion(true)
            
            let alert = UIAlertController(title: "Deleting category", message: "Are you sure you want to delete category and all transactions in this category", preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "Delete", style: .default ,handler: {[self] _ in
                self.context.delete(category)
                do {
                    try self.context.save()
                } catch {
                    abort()
                }
                self.getAllCategories()
            }))
            alert.addAction(UIAlertAction(title: "Cancel", style: .cancel,handler: nil))
            self.present(alert, animated: true)
        }
        action.backgroundColor = .systemRed
        action.image = UIImage(systemName: "trash")
        return action
    }
    // MARK: - Editing category
    /// Edits category name
    ///
    /// - Parameter indexPath: which row
    func editCategory(at indexPath:IndexPath){
        let category = categories[indexPath.row]
        let alert = UIAlertController(title: "Edit", message: "Edit category", preferredStyle: .alert)
        alert.addTextField(configurationHandler: nil)
        alert.addAction(UIAlertAction(title: "Edit", style: .default,handler: { _ in
            guard let field = alert.textFields?.first, let text = field.text, !text.isEmpty else {
                return
            }
            category.name = text
            do{
                try self.context.save()
            } catch {
                abort()
            }
            self.getAllCategories()
        }))
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel,handler: nil))
        present(alert, animated: true)
    }
    
    /// Shows edit action on swipe
    ///
    func editCategoryAction(at indexPath:IndexPath) -> UIContextualAction {
        let action = UIContextualAction(style: .normal, title: "Edit"){
            (action,view,completion) in completion(true)
            self.editCategory(at: indexPath)
        }
        action.backgroundColor = .systemTeal
        action.image = UIImage(systemName: "square.and.pencil")
        return action
    }

}
