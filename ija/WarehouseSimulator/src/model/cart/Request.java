package model.cart;

import java.util.ArrayList;

/**
 * Class Request
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class Request {

    private ArrayList<RequestItem> requestedList;

    public Request(ArrayList<RequestItem> list) {
        this.requestedList = list;
    }

    public ArrayList<RequestItem> getRequestedList() {
        return requestedList;
    }


    /**
     * print request
     */
    public void printRequest() {
        System.out.println("-------Vypis pozadavku-----");
        for (RequestItem item: requestedList) {
            System.out.println(item.getName() + " " + item.getRequestedQuantity());
        }
    }
    /**
     * print request and quantity in vehicle
     */
    public void printRequestVehicle() {
        System.out.println("----Obsah pozadavku po vyzvednuti----");
        for (RequestItem item: requestedList) {
            System.out.println(item.getName() + " Potrebuji jeste: " + item.getRequestedQuantity() + " Mam ve voziku " + item.getInVehicle());
        }
    }


}