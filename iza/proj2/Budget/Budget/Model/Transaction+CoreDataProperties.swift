//
//  Transaction+CoreDataProperties.swift
//  Budget
//

import Foundation
import CoreData


extension Transaction {

    @nonobjc public class func fetchRequest() -> NSFetchRequest<Transaction> {
        return NSFetchRequest<Transaction>(entityName: "Transaction")
    }

    @NSManaged public var amount: Int64
    @NSManaged public var date: Date
    @NSManaged public var expense: Bool
    @NSManaged public var name: String?
    @NSManaged public var category: Category?

}

extension Transaction : Identifiable {
    
}
