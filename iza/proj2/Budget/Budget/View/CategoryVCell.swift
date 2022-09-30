//
//  CategoryVCell.swift
//  Budget
//


import UIKit

class CategoryVCell: UITableViewCell {
    
    @IBOutlet weak var categoryName: UILabel!
    func setName(name:String){
        categoryName.text = name
    }
     
}
