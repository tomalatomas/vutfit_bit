//
//  MonthTransactionVCell.swift
//  Budget
//


import UIKit

class MonthTransactionVCell: UITableViewCell {

    @IBOutlet var amountLbl: UILabel!
    @IBOutlet var categoryLbl: UILabel!
    @IBOutlet var dateLbl: UILabel!
    @IBOutlet var nameLbl: UILabel!
    @IBOutlet var transImage: UIImageView!
    
    override func awakeFromNib() {
        super.awakeFromNib()
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)
    }

}
