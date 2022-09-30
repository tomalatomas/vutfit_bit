//
//  SummaryVC.swift
//  Budget
//

import UIKit
import CoreData

class SummaryVC: UIViewController {
    
    @IBOutlet var expensesLabel: UILabel!
    @IBOutlet var balanceLabel: UILabel!
    @IBOutlet var incomeLabel: UILabel!
    
    @IBOutlet var progressCircle: ProgressCircle!
    @IBOutlet var tableView: UITableView!
    
    // Array with transactions
    var monthTransactions:[Transaction] = []
    var currentMonthOffset = 0
    var monthlyBudget = 0
    let userDefaults = UserDefaults.standard

    override func viewDidLoad() {
        super.viewDidLoad()
        monthTransactions = getMonthTransactions()
        tableView.dataSource = self
        tableView.delegate = self
        tableView.tableFooterView = UIView()
        tableView.layer.cornerRadius=20

    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        currentMonthOffset = 0
        monthTransactions = getMonthTransactions()
        DispatchQueue.main.async {
            self.tableView.reloadData()
        }
        getMonthlyBudget()
        getStats()
    }
    
    @IBAction func nextMonth(_ sender: Any) {
        currentMonthOffset = currentMonthOffset+1
        monthTransactions = getMonthTransactions()
        DispatchQueue.main.async {
            self.tableView.reloadData()
        }
        getStats()
    }
    
    @IBAction func previousMonth(_ sender: Any) {
        currentMonthOffset = currentMonthOffset-1
        monthTransactions = getMonthTransactions()
        DispatchQueue.main.async {
            self.tableView.reloadData()
        }
        getStats()
    }
    
    /// Returns this months transactions
    ///
    /// - Returns: array with transactions
    func getMonthTransactions() -> [Transaction]{
        do{
            let request = Transaction.fetchRequest() as NSFetchRequest<Transaction>
            let endDay = Date().getMonthEnd(monthOffset: currentMonthOffset)
            let startDay = Date().getMonthStart(monthOffset: currentMonthOffset)
            let monthName = startDay.monthAsString()
            navigationItem.title = monthName
            //Filtering transactions of this month
            let predicate = NSPredicate(format: "date <= %@ AND date >= %@", endDay as CVarArg, startDay as CVarArg)
            request.predicate = predicate
            let sort = NSSortDescriptor(key: "date", ascending: false)
            request.sortDescriptors = [sort]
            return try MOC().fetch(request)
        } catch {
            abort()
        }
    }
    
    /// Stores user's monthly budget into variable
    ///
    func getMonthlyBudget() {
        let budgetKey = "budgetMonth"
        self.monthlyBudget = userDefaults.integer(forKey: budgetKey)
    }
    
    /// Fills info into stat labels and progress circle
    ///
    func getStats(){
        let currency = UserDefaults.standard.string(forKey: "currency") ?? ""
        
        var expenses = 0;
        var income = 0;
        for trans in monthTransactions {
            if trans.expense == true{
                expenses += Int(trans.amount)
            } else {
                income += Int(trans.amount)
            }
        }
        expensesLabel.text = String(expenses) + " " + currency
        incomeLabel.text = String(income) + " " + currency
        getMonthlyBudget()
        let balance = -expenses+income + monthlyBudget
        balanceLabel.text = String(balance) + " " + currency
        
        let totalIncome = monthlyBudget + income
        let percentage: Double = Double(balance) / Double(totalIncome)
        progressCircle.progress = CGFloat(percentage)
        
        progressCircle.backgroundColor = .lightGray
        progressCircle.color = .black

        if(percentage < 0.35) {

            if (percentage < 0.15){
                progressCircle.color = .systemRed
                if (percentage < 0.0){
                    progressCircle.backgroundColor = .systemRed
                }
            } else {
                progressCircle.color = .systemOrange
            }
        }
        
        
    }
    
}
// MARK: - Table view
extension SummaryVC: UITableViewDelegate, UITableViewDataSource {

    /// Sets number of section in table view
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Returns: number of sections
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1;

    }

    /// Sets number of rows in section
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: section to configure
    /// - Returns: number of row in section
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        switch section {
        case 0: return monthTransactions.count
        default: return 0
        }
    }

    /// Sets table view cell into tableview
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter indexPath: for which row andsection to configure cell
    /// - Returns: table view cell
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        switch indexPath.section {
        //Transactions
        case 0:
            let transCell = tableView.dequeueReusableCell(withIdentifier: "MonthTransactionCell", for: indexPath) as! MonthTransactionVCell
            let transaction = monthTransactions[indexPath.row]
            
            let currency = userDefaults.string(forKey: "currency") ?? ""
            
            transCell.amountLbl.text = String(transaction.amount) + " " + currency
            
            let dateFormatter = DateFormatter()
            dateFormatter.dateStyle = .medium
            let timeString = dateFormatter.string(from:transaction.date)
            transCell.dateLbl.text = timeString
            
            transCell.nameLbl.text = transaction.name

            if transaction.expense {
                transCell.transImage.image = UIImage(named: "expenseLogo")
            } else {
                transCell.transImage.image = UIImage(named: "incomeLogo")
            }
            transCell.categoryLbl.text = transaction.category?.name
            transCell.selectionStyle = .none
            return transCell
        default: break
        }
        return UITableViewCell()
    }
    
    /// Sets height of table header
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: for what section to configure the header
    /// - Returns: height of section header
    func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        switch section {
        case 0:
            return 30.0
        default:
            return tableView.sectionHeaderHeight
        }
    }
    /// Sets header of tableview
    ///
    /// - Parameter tableView: which tableView to configure
    /// - Parameter section: for what section to configure the header
    /// - Returns: header of section
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        switch section {
        case 0:
            let view = UIView()
            view.backgroundColor = .black
            let title = UILabel()
            title.font = UIFont.boldSystemFont(ofSize: 16)
            title.textColor = .white
            title.text = "Transactions of this month"
            view.addSubview(title)
            title.translatesAutoresizingMaskIntoConstraints = false
            title.centerYAnchor.constraint(equalTo: view.centerYAnchor).isActive = true
            title.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 0).isActive = true
            title.rightAnchor.constraint(equalTo: view.rightAnchor, constant: 0).isActive = true
            title.topAnchor.constraint(equalTo: view.topAnchor, constant: 5).isActive = true
            title.textAlignment = .center
            title.layer.cornerRadius = 20
            return view
        default: return UIView();
        }
    }
}

// MARK: - Date
extension Date {
    /// Returns date with first day in month
    ///
    /// - Returns: first date of  month
    func getMonthStart(monthOffset:Int) -> Date {
        let components:NSDateComponents = Calendar.current.dateComponents([.year, .month], from: self) as NSDateComponents
        components.month = Calendar.current.component(.month, from: Date()) + monthOffset
        components.day = 1
        return Calendar.current.date(from: components as DateComponents)!
    }
    /// Returns date with last day in month
    ///
    /// - Returns: last date of month
    func getMonthEnd(monthOffset:Int) -> Date {
        let components:NSDateComponents = Calendar.current.dateComponents([.year, .month], from: self) as NSDateComponents
        components.month += 1 + monthOffset
        components.day = 1
        components.day -= 1
        return Calendar.current.date(from: components as DateComponents)!
    }
    
    func monthAsString() -> String {
            let df = DateFormatter()
            df.setLocalizedDateFormatFromTemplate("LLLL yyyy")
            return df.string(from: self)
    }
}
