//
//  Simulator.swift
//  Simulator
//
//  Created by Filip Klembara on 17/02/2020.
//

import FiniteAutomata

/// Simulator
public struct Simulator {
    /// Finite automata used in simulations
    private let finiteAutomata: FiniteAutomata

    /// Initialize simulator with given automata
    /// - Parameter finiteAutomata: finite automata
    public init(finiteAutomata: FiniteAutomata) {
        self.finiteAutomata = finiteAutomata
    }

    /// Simulate automata on given string
    /// - Parameter string: string with symbols separated by ','
    /// - Returns: Empty array if given string is not accepted by automata,
    ///     otherwise array of states
    public func simulate(on string: String) -> [String] {
        var path : [String] = []
        path.append(finiteAutomata.initialState)
        // If automata initial state is final state and string is empty, return initial state
        if  finiteAutomata.finalStates.contains(finiteAutomata.initialState) && string.isEmpty{
            return path
        }
        if string.isEmpty {return []}
        let stringSymb = string.split(separator: ",")
        //2D array to store alternative possible paths
        //If current possible path doesnt accept string, algorithm takes latest posible path from this array and continues searching 
        var crossroads:[[String]] = []
        //Going through symbols in string
        var i : Int = -1
        while i<stringSymb.count {
            i+=1
            //Checking if we have gone through all the symbols
            if i==stringSymb.count {
                //We are at the end of the string, check if we can accept or not
                if finiteAutomata.finalStates.contains(path.last!){
                    //We are in final state, return path
                    return path
                } else {
                    // We have gone trough the whole string but we are not in final state, must find other path
                    if crossroads.isEmpty{
                        //There are no other ways to go, string is not accepted
                        return []
                    }
                    path = crossroads.removeLast()
                    i = path.count-1
                    continue
                }
                
            }
            // We are not at the end of the string
            //Finding possible pathways
            var possiblePaths : [[String]] = []
            for transition in finiteAutomata.transitions{
                //If we can use this transition, we append it into possible pathways
                if transition.from == path[path.count-1] && transition.with == stringSymb[i] {
                    //Append current path + next state into possible paths
                    var possiblePath = path
                    possiblePath.append(transition.to)
                    possiblePaths.append(possiblePath)
                }
            }
            if possiblePaths.isEmpty {
                //There are no possible pathways, we must go back and take different path
                if crossroads.isEmpty{
                    //There are no other ways to go, string is not accepted
                    return []
                }
                // We load possible path from crossroads and store it into path
                path = crossroads.removeLast()
                i = path.count-1
                //Starting searching new path
                continue
            } else {
                //Next path is first possible path
                path = possiblePaths.removeFirst()
                //Other possible paths store into crossroads
                if possiblePaths.isEmpty == false {
                    crossroads.append(contentsOf:possiblePaths)
                }
            }
                //We found possible pathways, pick the first one and rest add to crossroads - possible pathways if current path doesnt work out
        }
        //Check if found path has right amout of states and if last state is final
        if(path.count != stringSymb.count+1 || !finiteAutomata.finalStates.contains(path.last!)){
            return []
        }
        return path
    }
}
