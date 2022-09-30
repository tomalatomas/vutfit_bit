package model.item;

import model.cell.Shelf;

import java.lang.reflect.Array;
import java.util.ArrayList;
/**
 * Class InventoryItem
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class InventoryItem {
    private String name;
    private int inStock=0;
    private int requestedQuantity=0;
    private ArrayList<Integer> inShelfs = new ArrayList<>();

    public InventoryItem(String name){
        this.name = name;
    }


    public int getRequestedQuantity() {
        return requestedQuantity;
    }

    public void setRequestedQuantity(int requestedQuantity) {
        this.requestedQuantity = requestedQuantity;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getInStock() {
        return inStock;
    }

    public void addInStock(int inStock) {
        this.inStock += inStock;
    }

    public void removeFromStock(int inStock) {
        this.inStock -= inStock;
    }

    public ArrayList<Integer> getInShelfs() {
        return inShelfs;
    }

    /**
     * Add id of shelf, where item is
     * @param id id of shelf
     */
    public void inNewShelf(int id) {
        if (getInShelfs().contains(id)) return;
        this.inShelfs.add(id);
    }


    public void removeFromShelf(int id) {
        if (getInShelfs().contains(id)) this.inShelfs.remove(id);
    }
}
