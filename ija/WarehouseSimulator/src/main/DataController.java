package main;

import java.util.*;
import java.io.FileReader;
import java.io.IOException;

import model.cell.Cell;
import model.cell.LoadingPlace;
import model.cell.Path;
import model.cell.Shelf;
import model.item.InventoryItem;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

/**
 * Class DataController
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class DataController {
    //Creates 16x16 matrix that represents map
    private Cell[][] map = new Cell[16][16];
    private HashMap<String, Integer> itemList = new HashMap<>();
    private ArrayList<InventoryItem> inventoryList = new ArrayList<>();
    /**
     * Returns map of cells
     * @return map - map with cells
     */
    public Cell[][] getMap() {
        return this.map;
    }
    /**
     * Returns inventory list
     * @return  invetoryList - inventory items
     */
    public ArrayList<InventoryItem> getInventorylist() {
        return this.inventoryList;
    }

    public void insertCellIntoMap(Cell cell, int posX, int posY) {
        map[posY][posX] = cell;
    }
    /**
     * Returns instance of cell in the map at specified position
     * @param posX - Position in x axis of map
     * @param  posY - Position in y axis of map
     */
    public Cell getCellAtIndex(int posX, int posY) {
        return this.map[posY][posX];
    }

    /**
     * Checks if inventory already has information about item
     * @param name - item to check
     */
    public InventoryItem containsInventoryItem(String name){
        for(InventoryItem item : inventoryList) {
            if (item.getName().equals(name)) {
                return item;
            }
        }
        return null;
    }

    /**
     * Retrieves information from JSON file
     * @param pathToJSON - name of JSON file
     */
    public void getData(String pathToJSON){

        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                map[i][j] = new Path();
            }
        }
        JSONParser parser = new JSONParser();
        try  {
            JSONObject jsonObject = (JSONObject) parser.parse(new FileReader(pathToJSON));
            // SHELVES
            JSONArray shelves = (JSONArray) jsonObject.get("shelf");
            Iterator<String> iterator = shelves.iterator();
            while (iterator.hasNext()) {
               // int position  = iterator.next() != null ? iterator.next().intValue() : null;
                int position= Integer.parseInt(String.valueOf(iterator.next()));
                int posY = (Integer) position/16;
                int posX = (Integer) position%16;
                insertCellIntoMap(new Shelf(position),posX,posY);
            }
            JSONArray loadPlaces = (JSONArray) jsonObject.get("loadingPlace");
            iterator = loadPlaces.iterator();
            while (iterator.hasNext()) {
                int position= Integer.parseInt(String.valueOf(iterator.next()));
                int posY = (Integer) position/16;
                int posX = (Integer) position%16;
                if(getCellAtIndex(posX,posY) instanceof Shelf){
                    //ERROR LOADING PLACE IN SHELF
                }
                insertCellIntoMap(new LoadingPlace(),posX,posY);
            }

            JSONArray shelfContents = (JSONArray) jsonObject.get("shelfContents");
            Iterator<JSONObject>  shelfIterator = shelfContents.iterator();
            while (shelfIterator.hasNext()) {
                JSONObject shelf = shelfIterator.next();
                int id = Integer.parseInt(String.valueOf(shelf.get("id")));
                int posY = (Integer) id/16;
                int posX = (Integer) id%16;
                Cell cell = getCellAtIndex(posX, posY);
                if(!(cell instanceof Shelf)){
                    //ERROR SAVING contents to nonshelf
                }
                JSONArray content = (JSONArray) shelf.get("content");
                Iterator<JSONObject>  contentIterator = content.iterator();

                while (contentIterator.hasNext()) {
                    JSONObject contentOfShelf = contentIterator.next();
                    String name = (String) contentOfShelf.get("name");
                    int quantity = Integer.parseInt(String.valueOf(contentOfShelf.get("quantity")));
                    if(cell instanceof Shelf){
                        Shelf shelfCell = (Shelf) cell;
                        shelfCell.put(name, quantity);
                        InventoryItem inventoryItem = containsInventoryItem(name);
                        if (inventoryItem == null){
                            inventoryItem = new InventoryItem(name);
                            inventoryList.add(inventoryItem);
                        }
                        inventoryItem.inNewShelf(id);
                        inventoryItem.addInStock(quantity);
                    }
                }

            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }

    }

}
