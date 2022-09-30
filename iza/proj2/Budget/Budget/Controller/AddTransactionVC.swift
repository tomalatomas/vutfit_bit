//
//  AddTransactionVC.swift
//  Budget
//


import UIKit

class AddTransactionVC: UIViewController {

    @IBOutlet weak var segmentedCtrl: UISegmentedControl!
    @IBOutlet weak var amountField: UITextField!
    @IBOutlet weak var nameField: UITextField!
    @IBOutlet weak var datePicker: UIDatePicker!
    @IBOutlet weak var catPicker: UIPickerView!
    
    let context = (UIApplication.shared.delegate as! AppDelegate).persistentContainer.viewContext
    private var categories  = [Category]()
    
    
    /// Creates new transaction with details taken from view fields
    ///
    @IBAction func addTransaction(_ sender: Any) {
        
        guard let amount = amountField.text else {
            let alert = UIAlertController(title: "Amount", message: "Please insert amount", preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "Continue", style: .cancel,handler: nil))
            present(alert, animated: true)
            return
        }
        let amountInt = Int(amount) ?? 0
        
        let newTransaction = Transaction(context:context)
        newTransaction.amount = Int64(amountInt)
        
        let catNumber = catPicker.selectedRow(inComponent: 0)
        let category = categories[catNumber]
        newTransaction.category = category
        
        let date = datePicker.date
        newTransaction.date = date
        let name =  nameField.text
        newTransaction.name = name
        
        let expense = segmentedCtrl.selectedSegmentIndex
        
        switch expense {
        case 0: newTransaction.expense = true
        default: newTransaction.expense = false
        }
        do{
            try context.save()
        } catch {
            abort()
        }
        
        //Clearing view
        
        amountField.text = ""
        nameField.text = ""
    }
    
    override func viewDidAppear(_ animated: Bool) {
        setupCatPicker()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupCatPicker()
        let tap = UITapGestureRecognizer(target: self, action: #selector(UIInputViewController.dismissKeyboard))
        view.addGestureRecognizer(tap)
    }
    @objc func dismissKeyboard() {
        view.endEditing(true)
    }
    
    func setupCatPicker(){
        catPicker.delegate = self
        catPicker.dataSource = self
        do {
            categories = try context.fetch(Category.fetchRequest())
        } catch {
            abort()
        }
    }
    


}

extension AddTransactionVC : UIPickerViewDelegate, UIPickerViewDataSource {
    
    /// Sets number of components in pickerView
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Returns: number of components
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    /// Sets number of rows in pickerView
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Returns: number of rows
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return categories.count
    }
    
    
    /// Sets row label for row in pickerView
    ///
    /// - Parameter pickerView: which pickerView to configure
    /// - Parameter row: which row to configure
    /// - Parameter component: in which component
    /// - Returns: name of category at row index
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return categories[row].name
    }
    
}

