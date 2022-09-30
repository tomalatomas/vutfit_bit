package model.cell;

import model.item.ShelfItem;

import java.util.ArrayList;
import java.util.Hashtable;
/**
 * Class Shelf
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class Shelf extends Cell {
    private ArrayList<ShelfItem> content = new ArrayList<>();
    int id;
    public Shelf(int id) {
        super();
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public ArrayList<ShelfItem> getContent() {
        return content;
    }

    public void setContent(ArrayList<ShelfItem> content) {
        this.content = content;
    }

    /**
     * Put item in shelf
     * @param name name of item
     * @param quantity quantity of item
     */
    public void put(String name, int quantity) {
        ShelfItem temp = findItem(name);
        // item is not new new
        if(temp != null) {
            int oldQuantity = temp.getQuantity();
            temp.setQuantity(oldQuantity + quantity);
        } else {
            // item is new in shelf
            ShelfItem item = new ShelfItem(name, quantity);
            content.add(item);
        }
    }

    /**
     * Return true if shelf contains item, false if shelf doesnt contains intem
     * @param name
     * @return result of containning item
     */
    public boolean containsItem(String name) {
        for (ShelfItem item: content) {
            if (item.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }
    /**
     * Find item in shelf
     * @param name - name of finding item
     * @return shelfItem
     */
    private ShelfItem findItem(String name) {

        for (ShelfItem item: content) {
            if (item.getName().equals(name)) {
                return item;
            }
        }
        return null;
    }
    /**
     * Remove quantity or item from shelf
     * @param name - name of removed item
     * @param  quantity - quantity, how many we are removing
     */
    public int removeItemFromShelf(String name, int quantity) {
        int result = 0;
        int index = 0;
        ArrayList<ShelfItem> tmp = new ArrayList<>(content);
        for (ShelfItem item: content) {
            // is item what request want
            if (item.getName().equals(name)) {
                int itemQ = item.getQuantity();
                result = itemQ - quantity;
                // quantity from request is bigger then what is in shelf
                if (result <= 0) {
                    item.setQuantity(0);
                    result = quantity - itemQ;
                }
                else {
                    // shelf have more quantity then quantity from request
                    item.setQuantity(itemQ - quantity);
                    result = 0;

                }

                if (item.getQuantity() == 0) {
                    // remove item from shelf
                    tmp.remove(index);
                }

            }
            index++;
        }
        return result;
    }

    public boolean isEmpty(){
        return this.content.isEmpty();
    }

    /**
     * Print info about shelf
     */
    public void printShelf() {
        for (ShelfItem item: content) {
            System.out.println(item.getName() + " " + item.getQuantity());
        }
    }

}
