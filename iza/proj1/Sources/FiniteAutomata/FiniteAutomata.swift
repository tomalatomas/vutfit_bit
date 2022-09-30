//
//  FiniteAutomata.swift
//  FiniteAutomata
//
//  Created by Filip Klembara on 17/02/2020.
//

/// Finite automata

//https://matteomanferdini.com/codable/

public struct Transition {
    //Structure representing nested object: transition
    public let with: String
    public let to: String
    public let from: String
}

extension Transition: Decodable {
    enum CodingKeys: String, CodingKey {
        //Coding keys for transition, same as JSON, doesnt need raw values
            case with
            case to
            case from
    }
}

public struct FiniteAutomata {
    //Structure that represents characteristics of finite automata
    public let states: [String]
    public let symbols: [String]
    public let initialState: String
    public let finalStates: [String]
    public let transitions: [Transition]
}

extension FiniteAutomata: Decodable {
    enum CodingKeys: String, CodingKey {
        //Coding keys for automata, same as JSON, doesnt need raw values
        case states
        case symbols
        case initialState
        case finalStates
        case transitions
    }
}




