//
//  main.swift
//  proj1
//
//  Created by Filip Klembara on 17/02/2020.
//

import Foundation
import FiniteAutomata
import Simulator

func checkAutomata(finiteAutomata:FiniteAutomata, string:String) -> Result<Void, RunError> {
    //Checking that initial state is declared state
    if finiteAutomata.states.contains(finiteAutomata.initialState) == false {
            return .failure(.undefStateErr)
        }
    //Checking that all final states are declared
    for finState in finiteAutomata.finalStates{
        if finiteAutomata.states.contains(finState) == false {
            return .failure(.undefStateErr)
        }
    }
    //Checking that all states in transitions are declared
    for transition in finiteAutomata.transitions{
        //For every transition checking if variables "from" and "to" contain only declared states 
            // and if "to" variable contains only declared symbols 
        if (finiteAutomata.states.contains(transition.from) && finiteAutomata.states.contains(transition.to)) == false {
            return .failure(.undefStateErr)
        }
        if finiteAutomata.symbols.contains(transition.with) == false {
            return .failure(.undefSymbErr)
        }
    }
    //Check that string contains only declared symbols
    let stringSymbols = string.split(separator: ",")
    for symbol in stringSymbols {
        if finiteAutomata.symbols.contains(String(symbol)) == false {
            return .failure(.notAccepted)
        }
    }
    return .success(())
}

// MARK: - Main
func main() -> Result<Void, RunError> {
    //Checking count of arguments
    if CommandLine.argc != 3 {
        return .failure(.invalidArgs)
    }
    //Loading arguments to path
    let arguments = CommandLine.arguments // string;automataPath
    //Loading contents of json file
    do {
        // Get the json contents
        let content = try String(contentsOfFile: arguments[2], encoding: .utf8)
        //print(contents)
        do{
            //Decoding JSON
            let jsonData = content.data(using: .utf8)!
            let automata:FiniteAutomata = try JSONDecoder().decode(FiniteAutomata.self,from: jsonData)
            //Checking automata for undeclared symbols and states
            let result = checkAutomata(finiteAutomata:automata, string: arguments[1])
            // If checkAutomata returned error, we return it
            switch result {
                case .success:
                    break
                case .failure:
                    return result
            }
            //print(automata.states)
            //Simulating automata running through a state sequence 
            let simulator = Simulator(finiteAutomata:automata)
            let stateSequence = simulator.simulate(on: arguments[1])
            if stateSequence.isEmpty {
                return .failure(.notAccepted)
            }
            for state in stateSequence{
                print(state)
            }
        }
    } catch {
        return .failure(.fileError)
    }
    return .success(())
}

// MARK: - program body
let result = main()

switch result {
case .success:
    break
case .failure(let error):
    var stderr = STDERRStream()
    print("Error:", error.description, to: &stderr)
    exit(Int32(error.code))
}
