package main;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.cell.Cell;
import model.cell.LoadingPlace;
import model.cell.Path;
import model.cell.Shelf;
import model.item.InventoryItem;

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

/**
 * Class securing functionality of manual adding to map
 * @author Tomáš Tomala (xtomal02)
 * @author Martin Mlýnek (xmlyne06)
 *
 */

public class ManualController implements Initializable {

    Controller controller = null;
    private Cell[][] map = new Cell[16][16];
    private Shelf lastShelf = null;
    private ArrayList<InventoryItem> inventoryList = new ArrayList<>();
    private boolean isLoadingPlace = false;
    @FXML
    private TextField LX;

    @FXML
    private TextField LY;

    @FXML
    private Button AddL;

    @FXML
    private TextField SX;

    @FXML
    private TextField SY;

    @FXML
    private Button AddS;

    @FXML
    private Button AddItem;

    @FXML
    private TextField NameOfItem;

    @FXML
    private TextField Count;

    @FXML
    private Label labelAdd;

    /**
     * Set map to path
     * @param location lokace
     * @param resources zdroje
     */
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                map[i][j] = new Path();
            }
        }
    }

    /**
     * Set new map in main window
     */
    public void setNewMap() {
        controller.buildMapManually(inventoryList, map);

    }

    /**
     * Getter for controller
     * @return controller
     */
    public Controller getController() {
        return controller;
    }

    /**
     * Setter for controller
     * @param controller controller to set
     */
    public void setController(Controller controller) {
        this.controller = controller;
    }

    /**
     * Add loading place to map
     * @param actionEvent button click
     */
    public void addLoadingPlace(ActionEvent actionEvent) {
            // if user doesnt add information, he will get alert
            if (LX.getText().equals("") || LY.getText().equals("")) {
                Alert alert = new Alert(Alert.AlertType.ERROR, "Add info to text fields", ButtonType.OK);
                alert.showAndWait();
            } else {
                try {
                    // integer validation
                    int x = Integer.parseInt(LX.getText());
                    int y = Integer.parseInt(LY.getText());
                    LoadingPlace loadingPlace = new LoadingPlace();
                    // matrix validation
                    if (x < 16 && y < 16) {
                        isLoadingPlace = true;
                        map[y][x] = loadingPlace;
                        setNewMap();
                    } else {
                        Alert alert = new Alert(Alert.AlertType.ERROR, "You are out of range", ButtonType.OK);
                        alert.showAndWait();
                    }



                } catch (NumberFormatException e) {
                    Alert alert = new Alert(Alert.AlertType.ERROR, "Insert number into loading place positions", ButtonType.OK);
                    alert.showAndWait();
                }
            }
        LX.setText("");
        LY.setText("");
    }

    /**
     * Add shelf to map
     * @param actionEvent button click
     */
    public void addShelf(ActionEvent actionEvent) {
        // checking if we add loading place, because carts are setted to random place
        if(!isLoadingPlace) {
            Alert alert = new Alert(Alert.AlertType.WARNING, "You must add loading place", ButtonType.OK);
            alert.showAndWait();
            return;
        }
        // checks emptinesss of textfields
        if (SX.getText().equals("") || SY.getText().equals("")) {
            Alert alert = new Alert(Alert.AlertType.ERROR, "Add info to shelf text fields", ButtonType.OK);
            alert.showAndWait();
        } else {
            try {
                // int validation
                int x = Integer.parseInt(SX.getText());
                int y = Integer.parseInt(SY.getText());
                int id = x + y * 16;
                Shelf shelf = new Shelf(id);
                // matrix size validation
                if (x < 16 && y < 16) {
                    lastShelf = shelf;
                    labelAdd.setText("You are adding to shelf X: " + x + " Y: " + y);
                    map[y][x] = shelf;
                    setNewMap();
                } else {
                    Alert alert = new Alert(Alert.AlertType.ERROR, "You are out of range", ButtonType.OK);
                    alert.showAndWait();
                }



            } catch (NumberFormatException e) {
                Alert alert = new Alert(Alert.AlertType.ERROR, "Insert number into shelf positions", ButtonType.OK);
                alert.showAndWait();
            }
        }
        SX.setText("");
        SY.setText("");
    }

    /**
     * Add item to shelf
     * @param actionEvent button click
     */
    public void addItem(ActionEvent actionEvent) {
        // checks if we add shelf
        if (lastShelf == null) {
            Alert alert = new Alert(Alert.AlertType.ERROR, "You didnt add a shelf", ButtonType.OK);
            alert.showAndWait();
            return;
        }
        String name = NameOfItem.getText();
        int quantity = 0;
        // validation of quantity
        try {
            quantity = Integer.parseInt(Count.getText());
        } catch (NumberFormatException e) {
            Alert alert = new Alert(Alert.AlertType.ERROR, "Insert number to quantity field", ButtonType.OK);
            alert.showAndWait();
            return;
        }
        // checking if we add this, into invetory list, duplicite values
        if (!name.equals("") && quantity != 0) {
            lastShelf.put(name, quantity);
            InventoryItem inventoryItem = containsInventoryItem(name);
            if (inventoryItem == null){
                inventoryItem = new InventoryItem(name);
                inventoryList.add(inventoryItem);
            }
            inventoryItem.inNewShelf(lastShelf.getId());
            inventoryItem.addInStock(quantity);
            setNewMap();
        } else {
            Alert alert = new Alert(Alert.AlertType.ERROR, "You inserted sign number or no number", ButtonType.OK);
            alert.showAndWait();
        }
        Count.setText("");
        NameOfItem.setText("");

    }

    /**
     * Checks item name in inventory ist
     * @param name name of item
     * @return inventory item of this name in list
     */
    public InventoryItem containsInventoryItem(String name){
        for(InventoryItem item : inventoryList) {
            if (item.getName().equals(name)) {
                return item;
            }
        }
        return null;
    }

}
