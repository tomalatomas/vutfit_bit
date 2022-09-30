package main;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextArea;
import javafx.scene.layout.Pane;
import model.cart.Cart;
import model.cart.RequestItem;
import model.cell.Cell;
import model.cell.Shelf;
import model.item.ShelfItem;

import java.net.URL;
import java.util.ResourceBundle;
/**
 * Class InfoController
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class InfoController implements Initializable {
    @FXML
    private TextArea textArea;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        textArea.setEditable(false);
    }
    /**
     * Displays window with shelf contents
     * @param cell - shelf
     */
    public void printInfo(Cell cell) {
        textArea.setText("SHELF ITEMS:\n");
        if (cell instanceof Shelf){
            int counter = 1;
            for (ShelfItem item : ((Shelf) cell).getContent()){
                textArea.appendText("Item no."+counter+" | Item name:"+item.getName()+" | Quantity:"+item.getQuantity()+"\n");
                counter++;
            }
        } else {

        }
    }

    /**
     * Print info about cart
     * @param cart cart to print
     */
    public void printInfo(Cart cart) {
        textArea.setText("CART ITEMS:\n");

            int counter = 1;
            for (RequestItem item : cart.getRequest().getRequestedList()){
                textArea.appendText("Item no."+counter+" | Item name:"+item.getName()+" | Quantity:"+item.getInVehicle()+"\n");
                counter++;
            }

    }
}
