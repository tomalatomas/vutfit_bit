//
//  SettingsVC.swift
//  Budget
//

import UIKit
import CoreData
private let reuseIdentifier = "SettingsCell"

class SettingsVC: UIViewController {
    
    let currencies = ["USD","EUR","CZK"]
    let screenWidth = UIScreen.main.bounds.width-10
    let screenHeight = UIScreen.main.bounds.height/2
    var selectedRow = 0
    // MARK: - Properties
    
    @IBOutlet weak var tableView: UITableView!
    override func viewDidLoad() {
        super.viewDidLoad()
        configureTable()
    }
    
    
    
    /// Configures tableview and tablecell for tableview to use
    ///
    func configureTable(){
        tableView.tableFooterView = UIView()
        tableView.rowHeight = 60
        tableView.register(SettingsCell.self, forCellReuseIdentifier: reuseIdentifier)
        setTableDelegates()
        view.addSubview(tableView)
        tableView.frame = view.frame
    }
    
    /// Sets data source, delegate for tableview to use
    ///
    func setTableDelegates(){
        tableView.delegate = self
        tableView.dataSource = self
        tableView.layer.cornerRadius=20
    }
}
// MARK: - Table view
extension SettingsVC : UITableViewDelegate, UITableViewDataSource {
    
    /// Sets number of section in table view
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Returns: number of sections
    func numberOfSections(in tableView: UITableView) -> Int {
        return 2;
    }
    
    /// Sets number of rows in section
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: section to configure
    /// - Returns: number of row in section
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        guard let section = SettingsSections(rawValue: section) else { return 0 }
        switch section {
        case .preferences: return PreferencesOptions.allCases.count
        case .general: return GeneralOptions.allCases.count
        }
    }
    
    /// Sets header of section
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: for what section to configure the header
    /// - Returns: header of section
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        let view = UIView()
        view.backgroundColor = .black
         
        let title = UILabel()
        title.font = UIFont.boldSystemFont(ofSize: 16)
        title.textColor = .white
        title.text = SettingsSections(rawValue: section)?.description
        view.addSubview(title)
        title.translatesAutoresizingMaskIntoConstraints = false
        title.centerYAnchor.constraint(equalTo: view.centerYAnchor).isActive = true
        title.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 0).isActive = true
        title.rightAnchor.constraint(equalTo: view.rightAnchor, constant: 0).isActive = true
        title.textAlignment = .center
        return view
    }
    
    /// Sets height of section header
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: for what section to configure the header
    /// - Returns: height of section header
    func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        return 30
    }
    
    /// Sets table view cell into tableview
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter indexPath: for which row and section to configure cell
    /// - Returns: table view cell
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: reuseIdentifier, for: indexPath) as! SettingsCell
        guard let section = SettingsSections(rawValue: indexPath.section) else { return UITableViewCell() }
        switch section {
        case .preferences:
            let preferences = PreferencesOptions(rawValue: indexPath.row)
            cell.textLabel?.text = preferences?.description
        case .general:
            let general = GeneralOptions(rawValue: indexPath.row)
            cell.textLabel?.text = general?.description
        }
        cell.selectionStyle = .none
        cell.layer.cornerRadius = 20
        return cell
        
    }
    
    /// Row was selected
    ///
    /// - Parameter tableView: in which tableview
    /// - Parameter indexPath: which row in which section
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let section = SettingsSections(rawValue: indexPath.section) else { return }
        switch section {
        case .preferences:
            switch indexPath.row {
            case 0:
                setCurrency()
            case 1:
                setBudget()
            default:
                break;
            }
        case .general:
            switch indexPath.row {
            case 0:
                deleteUserData()
            default:
                break;
            }
        }
    }
    
    /// Sets default currency
    ///
    func setCurrency(){
        //Picker view
        let vc = UIViewController()
        vc.preferredContentSize = CGSize(width: screenWidth, height: screenHeight)
        let pickerView = UIPickerView(frame: CGRect(x: 0, y: 0, width: screenWidth, height: screenHeight))
        pickerView.delegate = self
        pickerView.dataSource = self
        pickerView.selectRow(selectedRow, inComponent: 0, animated: true)
        vc.view.addSubview(pickerView)
        //pickerView.centerXAnchor.constraint(equalTo: vc.view.centerXAnchor).isActive = true
        pickerView.centerYAnchor.constraint(equalTo: vc.view.centerYAnchor).isActive = true
        let alert = UIAlertController(title: "Select Currency", message: "", preferredStyle: .actionSheet)
                
                alert.setValue(vc, forKey: "contentViewController")
                alert.addAction(UIAlertAction(title: "Cancel", style: .cancel, handler: { (UIAlertAction) in
                }))
                
                alert.addAction(UIAlertAction(title: "Select", style: .default, handler: { (UIAlertAction) in
                    self.selectedRow = pickerView.selectedRow(inComponent: 0)
                    let selected = Array(self.currencies)[self.selectedRow]
                    UserDefaults.standard.setValue(selected, forKey: "currency")
                    do{
                        try MOC().save()
                    } catch{
                        abort()
                    }
                }))
                
                self.present(alert, animated: true, completion: nil)

    }
    
    /// Sets default budget
    ///
    func setBudget(){
        let alert = UIAlertController(title: "Change monthly budget", message: "Enter new budget", preferredStyle: .alert)
        alert.addTextField { (textField) in
            textField.keyboardType = .numberPad
        }
        alert.addAction(UIAlertAction(title: "Confirm", style: .default ,handler: {_ in
            guard let textField = alert.textFields?.first, let text = textField.text, !text.isEmpty else {
                return
            }
            let monthlyBudgetKey = "budgetMonth"
            let budget = textField.text ?? "0"
            UserDefaults.standard.setValue(Int(budget), forKey: monthlyBudgetKey)
            //Save
            do{
                try MOC().save()
            } catch{
                abort()
            }
        }))
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel,handler: nil))
        present(alert, animated: true)
    }
    
    /// Deletes all user data
    ///
    func deleteUserData(){
        
        let alert = UIAlertController(title: "Deleting user data", message: "Are you sure you want to delete all user data?", preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "Delete", style: .default ,handler: { _ in
            let fetchRequestTrans = NSFetchRequest<NSFetchRequestResult>(entityName: "Transaction")
            fetchRequestTrans.includesPropertyValues = false
            let fetchRequestCats = NSFetchRequest<NSFetchRequestResult>(entityName: "Category")
            fetchRequestCats.includesPropertyValues = false
            do {
                let transactions = try MOC().fetch(fetchRequestTrans) as! [NSManagedObject]
                let categories = try MOC().fetch(fetchRequestCats) as! [NSManagedObject]
                for trans in transactions {
                    MOC().delete(trans)
                }
                for cat in categories {
                    MOC().delete(cat)
                }
                let appDelegate: AppDelegate? = UIApplication.shared.delegate as? AppDelegate
                let preloadedDataKey = "didPreloadData"
                let userDefaults = UserDefaults.standard
                userDefaults.setValue(false, forKey: preloadedDataKey)
                appDelegate?.preloadData()
                // Save Changes
                try MOC().save()
            } catch {
                abort()
            }
        }))
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel,handler: nil))
        self.present(alert, animated: true)
        
        
    }
    
}

extension SettingsVC: UIPickerViewDelegate, UIPickerViewDataSource{
    /// Sets number of components in picker view
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Returns: number of components
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    /// Sets number of rows in component
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Parameter component:  what component to configure
    /// - Returns: number of rows in component
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return currencies.count

    }
    /// Sets height of row in pickerView
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Parameter component: for what component to configure the row
    /// - Returns: height of row
    func pickerView(_ pickerView: UIPickerView, rowHeightForComponent component: Int) -> CGFloat {
        return 60
    }
    /// Configure row in pickerview
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Parameter row: row to configure
    /// - Parameter component: in which component
    /// - Returns: row in pickerView
    func pickerView(_ pickerView: UIPickerView, viewForRow row: Int, forComponent component: Int, reusing view: UIView?) -> UIView {
        let label = UILabel(frame: CGRect(x: 0, y: 0, width: screenWidth, height: 30))
        label.text = currencies[row]
        label.sizeToFit()
        return label
    }
    
}
