//
//  TransactionsVC.swift
//  Budget
//


import UIKit
import CoreData


/// Gets appDelegate
/// - Returns: application delegate
func APP() -> AppDelegate {
    return (UIApplication.shared.delegate as! AppDelegate)
}

/// Gets core data context
/// - Returns: core data context
func MOC() -> NSManagedObjectContext{
    return APP().persistentContainer.viewContext
}

class TransactionsVC: UITableViewController {
    
    var category : Category?
    var transactions:[Transaction] = []
    
    /// Gets transactions of category
    ///
    /// - Parameter category: of which category
    /// - Returns: array of transactions
    func getTransactions(of category: Category?) -> [Transaction]  {
        guard let loadedCategory = category else {
            return [Transaction]()
        }
        
        do{
            let request = Transaction.fetchRequest() as NSFetchRequest<Transaction>
            let predicate = NSPredicate(format: "category == %@", loadedCategory)
            request.predicate = predicate
            let sort = NSSortDescriptor(key: "date", ascending: false)
            request.sortDescriptors = [sort]
            return try MOC().fetch(request)
        } catch {
            abort()
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        transactions = getTransactions(of: category)
        tableView.tableFooterView = UIView()
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        transactions = getTransactions(of: self.category)
        DispatchQueue.main.async {
            self.tableView.reloadData()
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
        return transactions.count
    }

    /// Sets table view cell into tableview
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter indexPath: for which row and section to configure cell
    /// - Returns: table view cell
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let transaction = transactions[indexPath.row]
        let cell = tableView.dequeueReusableCell(withIdentifier: "TransactionCell", for: indexPath) as! TransactionVCell
        cell.selectionStyle = .none
        
        let currency = UserDefaults.standard.string(forKey: "currency") ?? ""

        cell.amountLbl.text = String(transaction.amount) + " " + currency
        
        let dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .medium
        let timeString = dateFormatter.string(from:transaction.date)
        cell.dateLbl.text = timeString

        cell.nameLbl.text = transaction.name
        
        if transaction.expense {
            cell.imageTrans.image = UIImage(named: "expenseLogo")
        } else {
            cell.imageTrans.image = UIImage(named: "incomeLogo")
        }
        return cell
    }
    // MARK: Swiping actions
    
    /// Shows possible actions when swiping left on table cell
    ///
    /// - Parameter tableView: in which tableView
    /// - Parameter indexPath: for which row and section to configure actions
    /// - Returns: possible actions
    override func tableView(_ tableView: UITableView, trailingSwipeActionsConfigurationForRowAt indexPath: IndexPath) -> UISwipeActionsConfiguration? {
        let edit = editTransactionAction(at: indexPath)
        let delete = deleteTransaction(at: indexPath)
        return UISwipeActionsConfiguration(actions: [delete,edit])
    }
    
    
    // MARK: - Deleting category
    
    /// Deletes transaction from category
    ///
    /// - Parameter indexPath: which row
    func deleteTransaction(at indexPath:IndexPath) -> UIContextualAction {
        let transaction = transactions[indexPath.row]
        let action = UIContextualAction(style: .destructive, title: "Delete"){
            (action,view,completion) in completion(true)
            MOC().delete(transaction)
            do {
                try MOC().save()
            } catch {
                //error
            }
            self.transactions = self.getTransactions(of: self.category)
            DispatchQueue.main.async {
                self.tableView.reloadData()
            }
        }
        action.backgroundColor = .systemRed
        action.image = UIImage(systemName: "trash")
        return action
    }

    // MARK: - Editing category
    /// Edits transaction of category
    ///
    /// - Parameter indexPath: which row
    func editTransaction(at indexPath:IndexPath){
        let transaction = transactions[indexPath.row]
        let _sb = UIStoryboard(name:"Main", bundle: nil)
        guard let _vc = _sb.instantiateViewController(identifier: "TransactionDetail") as? TransactionDetailVC else {abort()}
        //_vc.transactions = _vc.getTransactions(of: category)
        _vc.transaction = transaction
        self.navigationController?.pushViewController(_vc, animated: true)
    }
    
    /// Shows edit action on swipe
    ///
    func editTransactionAction(at indexPath:IndexPath) -> UIContextualAction {
        let action = UIContextualAction(style: .normal, title: "Edit"){
            (action,view,completion) in completion(true)
            self.editTransaction(at: indexPath)
        }
        action.backgroundColor = .systemTeal
        action.image = UIImage(systemName: "square.and.pencil")
        return action
    }
    




}
