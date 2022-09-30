//
//  SettingsSections.swift
//  Budget
//


enum SettingsSections : Int, CaseIterable, CustomStringConvertible{
    case preferences
    case general
    
    var description : String {
        switch self {
        case .preferences: return "User preferences"
        case .general: return "General"
        }
    }
}

enum PreferencesOptions : Int, CaseIterable, CustomStringConvertible{
    case currency
    case monthlyBudget
    
    var description : String {
        switch self {
        case .currency: return "Set currency"
        case .monthlyBudget: return "Set default monthly budget"
        }
    }
}

enum GeneralOptions : Int, CaseIterable, CustomStringConvertible{
    case clearData
    
    var description : String {
        switch self {
        case .clearData: return "Clear all user data"
        }
    }
}
