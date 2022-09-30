//
//  RunError.swift
//  proj1
//
//  Created by Filip Klembara on 17/02/2020.
//

enum RunError: Error {
    case notImplemented
    case unknownError
    case notAccepted
    case invalidArgs
    case fileError
    case decodeError
    case undefStateErr
    case undefSymbErr
}

// MARK: - Return codes
extension RunError {
    var code: Int {
        switch self {
        case .notAccepted:
            return 6
        case .invalidArgs:
            return 11
        case .fileError:
            return 12
        case .decodeError:
            return 20
        case .undefStateErr:
            return 21
        case .undefSymbErr:
            return 22
        case .notImplemented:
            return 66
        case .unknownError:
            return 99
        }
    }
}

// MARK:- Description of error
extension RunError: CustomStringConvertible {
    var description: String {
        switch self {
        case .notAccepted:
            return "String is not accepted by the automata"
        case .invalidArgs:
            return "Invalid arguments!"
        case .fileError:
            return "Error while working with the file!"
        case .decodeError:
            return "Error while decoding JSON!"
        case .undefStateErr:
            return "Automata contains undefined state"
        case .undefSymbErr:
            return "Automata contains undefined symbol!"
        case .notImplemented:
            return "Not implemented!"
        case .unknownError:
            return "Unknown error"
        }
    }
}
