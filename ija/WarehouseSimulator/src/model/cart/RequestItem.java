package model.cart;

import model.item.InventoryItem;

import java.util.ArrayList;
/**
 * Class RequestItem
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class RequestItem {

    private int inVehicle = 0;
    private InventoryItem item;

    public void setRequestedQuantity(int requestedQuantity) {
        this.requestedQuantity = requestedQuantity;
    }

    private int requestedQuantity;

    public RequestItem(InventoryItem invItem, int requestedQuantity) {
        //super(name);
        this.requestedQuantity = requestedQuantity;
        this.item = invItem;
    }

    public int getInVehicle() {
        return inVehicle;
    }

    public void setInVehicle(int inVehicle) {
        this.inVehicle = inVehicle;
    }


    public int getRequestedQuantity() {
        return requestedQuantity;
    }

    public ArrayList<Integer> getShelfList(){
        return this.item.getInShelfs();
    }

    public String getName(){
        return this.item.getName();
    }

    /**
     * Get first shelf, where item is
     * @return id of shelf
     */
    public Integer getShelfPosition(){
        ArrayList<Integer> shelfsList = this.item.getInShelfs();
        if (shelfsList.size() < 1){
            return -1; // ERROR, Item not found
        }
        return shelfsList.get(0);
    }
}
