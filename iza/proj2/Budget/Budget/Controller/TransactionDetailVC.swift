//
//  TransactionDetailVC.swift
//  Budget
//

import UIKit

class TransactionDetailVC: UIViewController{
    
    @IBOutlet var pickerView: UIPickerView!
    @IBOutlet var amountField: UITextField!
    @IBOutlet var segmentedCtrl: UISegmentedControl!
    @IBOutlet var nameField: UITextField!
    @IBOutlet var datePicker: UIDatePicker!
    private var categories  = [Category]()

    
    var transaction : Transaction?
    override func viewDidLoad() {
        super.viewDidLoad()
        navigationItem.largeTitleDisplayMode = .never
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Edit", style: .plain, target: self, action: #selector(editTapped))
        setupCatPicker()
        setupView()
        let tap = UITapGestureRecognizer(target: self, action: #selector(UIInputViewController.dismissKeyboard))
        view.addGestureRecognizer(tap)
    }
    
    /// Dismissed keyboard on tap
    ///
    @objc func dismissKeyboard() {
        view.endEditing(true)
    }
    
    
    /// Fills in fields with edited transaction information
    ///
    func setupView(){
        amountField.text = String(transaction?.amount ?? 0)
        nameField.text = transaction?.name
        datePicker.date = transaction?.date ?? Date()
        if transaction?.expense == true {
            self.segmentedCtrl.selectedSegmentIndex = 0
        } else {
            self.segmentedCtrl.selectedSegmentIndex = 1
        }
        
    }
    @objc func editTapped(_ sender: Any){
        guard let amount = amountField.text else {
            let alert = UIAlertController(title: "Amount", message: "Please insert amount", preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "Continue", style: .cancel,handler: nil))
            present(alert, animated: true)
            return
        }
        let amountInt = Int(amount) ?? 0
        self.transaction?.amount = Int64(amountInt)
         
        let catNumber = pickerView.selectedRow(inComponent: 0)
        let category = categories[catNumber]
        self.transaction?.category = category
        
        let date = datePicker.date
        self.transaction?.date = date
        let name =  nameField.text
        self.transaction?.name = name
        let expense = segmentedCtrl.selectedSegmentIndex
        switch expense {
        case 0: self.transaction?.expense = true
        default: self.transaction?.expense = false
        }
        do {
            try MOC().save()
        } catch {
            abort()
        }
        self.navigationController?.popViewController(animated: true)
    }
    
}

extension TransactionDetailVC : UIPickerViewDelegate, UIPickerViewDataSource {
    
    
    /// Sets data source and delegate for pickerView
    ///
    func setupCatPicker(){
        pickerView.delegate = self
        pickerView.dataSource = self
        do {
            categories = try MOC().fetch(Category.fetchRequest())
        } catch {
            abort()
        }
        let catIndex = categories.firstIndex{$0 === self.transaction?.category} ?? 0
        pickerView.selectRow(catIndex, inComponent: 0, animated: true)
    }
    
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
