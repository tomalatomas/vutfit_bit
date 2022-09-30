package main;

import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.control.cell.TextFieldTableCell;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.layout.Pane;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.stage.Window;
import model.cart.Cart;
import model.cart.PathInfo;
import model.cart.Request;
import model.cart.RequestItem;
import javafx.util.converter.IntegerStringConverter;
import javafx.scene.shape.Rectangle;
import javafx.scene.paint.Color;
import model.cell.Cell;
import model.cell.LoadingPlace;
import model.cell.Path;
import model.cell.Shelf;
import model.item.InventoryItem;


import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;
import java.util.stream.IntStream;


/**
 * Main controller
 * @author Tomáš Tomala (xtomal02)
 * @author Martin Mlýnek (xmlyne06)
 */
public class Controller extends Thread implements Initializable {
    final double SCALE_DELTA = 1.1;
    final int CART_SIZE = 100;
    @FXML
    private TableView<InventoryItem> tableview;

    @FXML
    private TableColumn<InventoryItem, String> nameCol;

    @FXML
    private TableColumn<InventoryItem, Integer> requestQuantityCol;

    @FXML
    private TableColumn<InventoryItem, Integer> inStockCol;

    @FXML
    private Pane mapPane;

    private ArrayList<Cart> carts = new ArrayList<>();

    ObservableList<InventoryItem> list = FXCollections.observableArrayList();
    static int simulationSpeed = 4;

    public static Cell[][] map = new Cell[16][16];
    public Rectangle[][] gMap = new Rectangle[16][16];
    public ArrayList<InventoryItem> inventoryItems = new ArrayList<>();

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        // setting columm names
        nameCol.setCellValueFactory(new PropertyValueFactory<InventoryItem, String>("name"));
        inStockCol.setCellValueFactory(new PropertyValueFactory<InventoryItem, Integer>("inStock"));
        requestQuantityCol.setCellValueFactory(new PropertyValueFactory<InventoryItem, Integer>("requestedQuantity"));
        tableview.setEditable(true);
        requestQuantityCol.setEditable(true);
        // setting string to int converter
        requestQuantityCol.setCellFactory(TextFieldTableCell.forTableColumn(new IntegerStringConverter() {
            @Override
            public Integer fromString(String value) {
                try {
                    return super.fromString(value);
                } catch(NumberFormatException e) {
                    Alert alert = new Alert(Alert.AlertType.ERROR, "Input numbers only", ButtonType.OK);
                    alert.showAndWait();
                    return 0;
                }
            }
        }));
        // set items to table
        tableview.setItems(list);
        //this.createCarts();
        this.showStartup();
        zoom();
    }

    /**
     * Set new edited value in table
     * @param requestItemIntegerCellEditEvent selected cell
     */
    public void onEditChange(TableColumn.CellEditEvent<RequestItem, Integer> requestItemIntegerCellEditEvent) {
        InventoryItem ri = tableview.getSelectionModel().getSelectedItem();
        int value = requestItemIntegerCellEditEvent.getNewValue();
        if (ri.getInStock() >= value) {
            ri.setRequestedQuantity(value);
        } else {
            Alert alert = new Alert(Alert.AlertType.ERROR, "Requested quantity not in stock", ButtonType.OK);
            alert.showAndWait();
            tableview.refresh();
        }
    }

    /**
     * Zooms into map
     */
    @FXML
    private void zoom() {
        mapPane.setOnScroll(new EventHandler<ScrollEvent>() {
            @Override public void handle(ScrollEvent event) {
                event.consume();

                if (event.getDeltaY() == 0) {
                    return;
                }
                double scaleFactor =
                        (event.getDeltaY() > 0)
                                ? SCALE_DELTA
                                : 1/SCALE_DELTA;

                mapPane.setScaleX(mapPane.getScaleX() * scaleFactor);
                mapPane.setScaleY(mapPane.getScaleY() * scaleFactor);
            }
        });
    }
    /**
     * Restarts the scene and shows startup screen
     */
    public void restart(){
        stopSim();
        showStartup();
    }
    /**
     * Increases simulation speed
     */
    public void fastForward(){
        this.simulationSpeed = simulationSpeed == 16 ? 4 : simulationSpeed*2 ;
    }
    /**
     * Shows startup screen
     */
    public void showStartup(){
        int toFill[] = {41,81,121,161,201,42,43,123,83,122,205,85,125,165,45,47,46,126,87,167,207,49,49,89,129,169,209,210,130,131,211,50,51,54,53,93,133,134,135,175,215,213,213,214,55,58,57,97,137,139,138,59,217,218,219,179,142,103,64,104,144,184,224,227,228,228,229,189,149,149,109,69,68,67,107,147,187,71,111,151,191,231,193,233,113,73,72,152,76,76,77,78,118,158,157,156,196,236,237,238,377,376,378,417,457,497,497,537,380,420,500,500,540,460,381,382,422,462,502,542,697,696,698,738,778,818,858,857,857,857,856,816,776,736,860,820,740,700,701,702,742,822,782,862,781,781,781,780,692,732,732,772,812,852,853,854,704,744,784,784,824,864,865,826,786,746,705,541,1014,1054,1094,1134,1134,1174,1055,1056,1016,1096,1136,1176,1178,1098,1058,1018,1019,1020,1060,1100,1140,1180,1138,1099,1022,1022,1062,1142,1182,1102,1023,1024,1064,1104,1103,1103};
        int size = 640;
        int cellYsize = size/40;
        int cellXsize = size/40;
        for (int i = 0; i < size; i+= cellXsize){
            int posX = i/cellXsize;
            for (int j = 0; j < size; j+= cellYsize){
                int posY = j/cellXsize;
                Rectangle r = new Rectangle(i,j,cellXsize,cellYsize);
                int position = posX+(posY*40);
                Color colorFill = Color.WHITE;
                if(IntStream.of(toFill).anyMatch(x -> x == position)){
                    colorFill = Color.BLACK;
                }
                Color colorStroke = Color.BLACK;
                r.setFill(colorFill);
                r.setStroke(colorStroke);
                mapPane.getChildren().add(r);
            }
        }
    }
    /**
     * Displays legend for the map
     */
    public void displayHelp(){
        stopSim();
        int blackFill[] = {47,46,46,45,85,165,125,205,206,207,209,169,129,89,50,91,131,171,211,130,49,51,53,93,93,133,213,173,54,55,95,134,175,215,57,58,59,98,138,178,218,
                287,286,285,325,365,405,445,447,446,289,329,369,409,449,450,451,293,333,373,413,413,453,454,415,455,375,335,295,294,299,298,297,377,337,378,379,419,459,458,457,301,341,381,421,421,461,462,463,423,383,343,303,305,345,385,425,465,306,347,307,386,427,467,309,349,389,429,469,471,470,390,391,310,311
                ,525,605,565,645,685,686,687,529,569,609,649,689,690,691,651,611,571,531,530,693,653,573,533,613,534,535,615,575,655,695,614,537,577,657,697,617,538,579,619,659,698
                ,546,545,544,584,624,625,626,666,706,705,704,548,588,628,668,708,550,551,552,591,631,671,711,714,674,594,554,634,555,556,636,635,715,716
                ,1051,1050,1049,1089,1129,1130,1131,1171,1211,1210,1209,1053,1093,1133,1133,1173,1213,1134,1135,1175,1175,1095,1055,1215,1057,1137,1177,1217,1097,1058,1059,1138,1139,1218,1219,1061,1101,1141,1181,1221,1222,1223,1065,1105,1145,1185,1225,1066,1067,1146,1147
                ,807,806,805,845,885,925,965,967,966,886,887,809,849,889,929,969,850,811,851,931,971,891,813,853,933,973,893,814,815,855,895,894,817,818,819,858,898,938,978,821,862,823,902,942,982,861,863
        };
        int yellowFill[] = {81,121,161,162,163,123,83,82,122};
        int greenFill[] = {561,601,641,642,643,603,603,563,562,602};
        int redFill[] = {321,322,323,363,362,361,401,402,403};
        int blueFill[] = {841,881,921,922,923,883,843,842,882};
        int grayFill[] = {661,701,702,662,1081,1121,1161,1162,1163,1123,1083,1082,1122};
        int size = 640;
        int cellYsize = size/40;
        int cellXsize = size/40;
        for (int i = 0; i < size; i+= cellXsize){
            int posX = i/cellXsize;
            for (int j = 0; j < size; j+= cellYsize){
                int posY = j/cellXsize;
                Rectangle r = new Rectangle(i,j,cellXsize,cellYsize);
                int position = posX+(posY*40);
                Color colorFill = Color.WHITE;
                if(IntStream.of(yellowFill).anyMatch(x -> x == position)){
                    colorFill = Color.YELLOW;
                } else if(IntStream.of(blackFill).anyMatch(x -> x == position)){
                    colorFill = Color.BLACK;
                } else if(IntStream.of(redFill).anyMatch(x -> x == position)){
                    colorFill = Color.RED;
                } else if(IntStream.of(blueFill).anyMatch(x -> x == position)){
                    colorFill = Color.LIGHTSTEELBLUE;
                } else if(IntStream.of(greenFill).anyMatch(x -> x == position)){
                    colorFill = Color.DARKSEAGREEN;
                }else if(IntStream.of(grayFill).anyMatch(x -> x == position)){
                    colorFill = Color.GRAY;
                }
                Color colorStroke = Color.BLACK;
                r.setFill(colorFill);
                r.setStroke(colorStroke);
                mapPane.getChildren().add(r);
            }
        }
    }

    /**
     * Loads first map from json
     */
    public void loadMap1(){
        stopSim();
        DataController datacntrl = new DataController();
        datacntrl.getData("data/warehouse1.json");
        this.map = datacntrl.getMap();
        this.inventoryItems = datacntrl.getInventorylist();
        for(InventoryItem item : inventoryItems) {
            list.add(item);
        }
        buildMap();
    }
    /**
     * Loads second map from json
     */
    public void loadMap2(){
        stopSim();
        DataController datacntrl = new DataController();
        datacntrl.getData("data/warehouse2.json");
        this.map = datacntrl.getMap();
        this.inventoryItems = datacntrl.getInventorylist();
        for(InventoryItem item : inventoryItems) {
            list.add(item);
        }
        buildMap();
    }
    /**
     * Takes information about warehouse loadMap functions and creates map
     */
    public void buildMap(){
        int size = 640;
        int cellYsize = size/16;
        int cellXsize = size/16;
        for (int i = 0; i < size; i+= cellXsize){
            int posX = i/40;
            for (int j = 0; j < size; j+= cellYsize){
                int posY = j/40;
                Rectangle r = new Rectangle(i,j,cellXsize,cellYsize);
                //Type and color of cell
                Color colorFill = Color.WHITE;
                Color colorStroke = Color.BLACK;
                if(this.map[posY][posX] instanceof Path){
                    if (map[posY][posX].isClosed()){
                        colorFill = Color.RED;
                    }
                }
                if(this.map[posY][posX] instanceof Shelf){
                    if (((Shelf) this.map[posY][posX]).isEmpty()){
                        colorFill = Color.LIGHTSTEELBLUE;
                    } else {
                        colorFill = Color.GREY;
                    }
                } else if(this.map[posY][posX] instanceof LoadingPlace){
                    int position = posX+(posY*16);
                    ((LoadingPlace) this.map[posY][posX]).createLoadingPlace(position);
                    colorFill = Color.DARKSEAGREEN;
                    if (map[posY][posX].isClosed()){
                        colorFill = Color.RED;
                    }
                }
                r.setOnMousePressed(event -> clickOnCell(event,posX,posY));
                r.setFill(colorFill);
                r.setStroke(colorStroke);
                gMap[posY][posX] = r;
                mapPane.getChildren().add(r);
            }
        }
        this.createCarts();
    }
    /**
     * Remove quantity or item from shelf
     */
    public void addRequest(ActionEvent actionEvent) {
        //ArrayList<RequestItem> newList = new ArrayList<>(list);
        //Loads requested items from inventory list
        ArrayList<RequestItem> requestList = new ArrayList<>();
        int requestedQuantityAll = 0;
        for (InventoryItem item: list) {
            int requestQuantity = item.getRequestedQuantity();
            requestedQuantityAll += item.getRequestedQuantity();
            if (requestQuantity < 0){
                Alert alert = new Alert(Alert.AlertType.ERROR, "Invalid request", ButtonType.OK);
                alert.showAndWait();
                return;
            } else if(requestQuantity> 0){
                if (requestQuantity > item.getInStock()){
                    Alert alert = new Alert(Alert.AlertType.ERROR, "Requested quantity not in stock", ButtonType.OK);
                    alert.showAndWait();
                    return;
                } else {
                    RequestItem reqItem = new RequestItem(item,requestQuantity);
                    requestList.add(reqItem);
                    item.removeFromStock(requestQuantity);
                }
            }
        }
        if(requestedQuantityAll > CART_SIZE){
            Alert alert = new Alert(Alert.AlertType.ERROR, "Requested quantity exceeds cart size ("+CART_SIZE+")!", ButtonType.OK);
            alert.showAndWait();
            return;
        }
        if(requestList.isEmpty()){
            Alert alert = new Alert(Alert.AlertType.ERROR, "Please insert requested items", ButtonType.OK);
            alert.showAndWait();
            return;
        }
        this.clearQuantityinTable();
        Request request = new Request(requestList);
        //request.printRequest();
        this.sendCartWithRequest(request);
    }

    /**
     * Resets quantity of items in table
     */
    public void clearQuantityinTable() {
        for (InventoryItem item: list) {
            item.setRequestedQuantity(0);
        }
        tableview.refresh();
    }

    /**
     * Creates carts for requests
     */
    public void createCarts() {
        for (int i = 0; i < 5; i++) {
            int position = LoadingPlace.getRandomLoadPlace();
            Cart cart = new Cart(position);
            carts.add(cart);
        }
    }

    /**
     * Gives cart request with items to execute
     * @param request - Request to execute
     */
    public void sendCartWithRequest(Request request) {
        Cart cart = findFreeCart();
        if (cart != null) {
            cart.setRequest(request);
            cart.setFree(false);
            executeCart(cart);
            return;
        }
        return;
    }
    /**
     * Returns first unused cart
     * @return cart - found cart
     */
    public Cart findFreeCart() {
        for (Cart cart: carts) {
            if (cart.isFree()) {
                return cart;
            }
        }
        Alert alert = new Alert(Alert.AlertType.INFORMATION, "All carts are in use", ButtonType.OK);
        alert.showAndWait();
        return null;
    }
    /**
     * Shows screen with summary of cart sent for request
     * @param cart - cart to send for items
     */
    public void endWindows(Cart cart) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("infoWindow.fxml"));
            Parent root = loader.load();
            Stage stage = new Stage();
            InfoController infoController = loader.getController();
            infoController.printInfo(cart);
            stage.setTitle("Cart information");
            stage.setScene(new Scene(root, 640, 320));
            stage.setResizable(false);
            stage.show();                }
        catch (IOException e) {
            e.printStackTrace();
        }
        cart.emptyCart();
        cart.setFree(true);
        this.refreshMap();

    }
    /**
     * Shows screen for manual input
     */
    public void manualWindow() {
        stopSim();
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("manualAdd.fxml"));
            Parent root = loader.load();
            Stage stage = new Stage();
            ManualController manualController = loader.getController();
            manualController.setController(this);
            stage.setTitle("Manual add");
            stage.setScene(new Scene(root, 375, 400));
            stage.setResizable(false);
            stage.show();
        }
        catch (IOException e) {
            e.printStackTrace();
        }

    }

    /**
     * Checks if carts is on position specified by params
     * @param posX positon X
     * @param posY position Y
     * @return Cart or null
     */
    private Cart cartOnPos(int posX, int posY){
        for (Cart cart: carts) {
            if (cart.getPosX() == posX && cart.getPosY() == posY && !cart.isFree()) {
                return cart;
            }
        }
        return null;
    }
    /**
     * Registering click into map
     * @param posX - Position in x axis of map
     * @param  posY - Position in y axis of map
     */
    private void clickOnCell(MouseEvent event, int posX, int posY) {
        int cellPosition = posX+(posY*16);
        Cart cart = cartOnPos(posX,posY);
        if (cart!= null && !cart.isFree()){
            try {
                ArrayList<Integer> remainingPath = cart.getRemainingPath();
                if(!remainingPath.isEmpty()) {
                    for (int pos : remainingPath) {
                        int pathPosX = pos % 16;
                        int pathPosY = pos / 16;
                        gMap[pathPosY][pathPosX].setStroke(Color.DARKORANGE);
                        gMap[pathPosY][pathPosX].setStrokeWidth(3);
                    }
                }
                FXMLLoader loader = new FXMLLoader(getClass().getResource("infoWindow.fxml"));
                Parent root = loader.load();
                Stage stage = new Stage();
                InfoController infoController = loader.getController();
                infoController.printInfo(cart);
                stage.setTitle("Cart information");
                stage.setScene(new Scene(root, 640, 320));
                stage.setResizable(false);
                stage.show();                }
            catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            if (map[posY][posX] instanceof Shelf){
                try {
                    FXMLLoader loader = new FXMLLoader(getClass().getResource("infoWindow.fxml"));
                    Parent root = loader.load();
                    Stage stage = new Stage();
                    InfoController infoController = loader.getController();
                    infoController.printInfo(map[posY][posX]);
                    stage.setTitle("Shelf information");
                    stage.setScene(new Scene(root, 640, 320));
                    stage.setResizable(false);
                    stage.show();                }
                catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (map[posY][posX] instanceof LoadingPlace){
                /*Color colorFill;
                if (map[posY][posX].isClosed()){
                    map[posY][posX].setClosure(false);
                    colorFill = Color.DARKSEAGREEN;
                    gMap[posY][posX].setFill(colorFill);
                } else {
                    map[posY][posX].setClosure(true);
                    redirectPaths(cellPosition);
                    colorFill = Color.RED;
                    gMap[posY][posX].setFill(colorFill);
                }*/
            }
            if (map[posY][posX] instanceof Path){
                Color colorFill;
                if (map[posY][posX].isClosed()){
                    map[posY][posX].setClosure(false);
                    colorFill = Color.WHITE;
                    gMap[posY][posX].setFill(colorFill);
                } else {
                    map[posY][posX].setClosure(true);
                    redirectPaths(cellPosition);
                    colorFill = Color.RED;
                    gMap[posY][posX].setFill(colorFill);
                }
            }

        }
    }
    /**
     * Redirects path when closure is in way
     * @param cellPosition - position of closure
     */
    private void redirectPaths(int cellPosition) {
        for (Cart cart : this.carts){
            if(!cart.isFree() && cart.getRemainingPath().contains(cellPosition)){
                cart.getThread().setStop(true);
                //Removes path discovery from map
                removePathDiscovery(cart);
                executeCart(cart);
            }
        }
    }
    /**
     * Removes highlited path of cart
     * @param cart - cart sent for items
     */
    private void removePathDiscovery(Cart cart) {
        ArrayList<Integer> path = cart.getRemainingPath();
        for (int pos : path) {
            //if(!isPathOfOtherCart(pos,cart)){
            if(true){
                int x = pos % 16;
                int y = pos / 16;
                gMap[y][x].setStroke(Color.BLACK);
                gMap[y][x].setStrokeWidth(1);
            }
        }
    }
    /**
     * Checks if position in map is in a way of some cart
     * @param currentCart - cart sent for items
     * @param pos - position in map
     */
    private boolean isPathOfOtherCart(int pos, Cart currentCart) {
        for(Cart cart  : carts){
            if(cart.equals(currentCart)) continue;
            if(cart.isFree()) continue;
            ArrayList<Integer> remainingPath = cart.getRemainingPath();
            if(remainingPath==null || remainingPath.isEmpty()) continue;
            for (int nextPosition : remainingPath){
                if(nextPosition == pos) return true;
            }
        }
        return false;
    }

    /**
     * Stops simulation
     */
    public void stopSim(){
        mapPane.getChildren().clear();
        map = null;
        gMap = null;
        map = new Cell[16][16];
        gMap = new Rectangle[16][16];
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                map[i][j] = new Path();
                gMap[i][j] = new Rectangle();
            }
        }
        this.removeThreads();
        inventoryItems.clear();
        carts.clear();
        list.clear();
        LoadingPlace.clearLoadingPlaces();
    }
    /**
     * Redraws map
     */
    public void refreshMap(){
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x< 16; x++) {
                if (map[y][x] instanceof Shelf){
                    if(((Shelf) map[y][x]).isEmpty()){
                        gMap[y][x].setFill(Color.LIGHTSTEELBLUE);
                    } else {
                        gMap[y][x].setFill(Color.GREY);
                    }
                }
                if (map[y][x] instanceof LoadingPlace){
                    if (map[y][x].isClosed()){
                        gMap[y][x].setFill(Color.RED);
                    } else {
                        gMap[y][x].setFill(Color.DARKSEAGREEN);
                    }
                }
                if (map[y][x] instanceof Path){
                    if (map[y][x].isClosed()){
                        gMap[y][x].setFill(Color.RED);
                    } else {
                        gMap[y][x].setFill(Color.WHITE);
                    }
                }
            }
        }
        for (int i = 0; i < carts.size(); i++) {
            Cart cart = carts.get(i);
            if(!cart.isFree()){
                int posX = cart.getPosX();
                int posY = cart.getPosY();
                Text text = new Text(""+i);
                text.setFill(Color.BLACK);
                gMap[posY][posX].setFill(Color.YELLOW);
            }
        }
    }

    /////////////////////////PATHFINDING/////////////////////////

    /**
     * Send cart for every requested item
     * @param cart - cart to send for items
     */
    public void executeCart(Cart cart){
        CartThread ct = new CartThread(this, cart);
        ct.start();
        cart.setThread(ct);
    }

    /**
     * Load map file in pc
     * @param actionEvent button click
     */
    @FXML
    public void loadMapFile(ActionEvent actionEvent) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open Resource File");
        Window window = ((Node) (actionEvent.getSource())).getScene().getWindow();
        // File finder window
        fileChooser.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("JSON files", "*.json")
        );
        File selectedFile = fileChooser.showOpenDialog(window);
        if (selectedFile != null) {
            String path = selectedFile.getPath();

            stopSim();
            DataController datacntrl = new DataController();
            datacntrl.getData(path);
            this.map = datacntrl.getMap();
            this.inventoryItems = datacntrl.getInventorylist();
            for(InventoryItem item : inventoryItems) {
                list.add(item);
            }
            buildMap();

        }

        actionEvent.consume();

    }

    /**
     * Build new map from manual add
     * @param inventoryList new inventory
     * @param map new map
     */
    public void buildMapManually(ArrayList<InventoryItem> inventoryList, Cell[][] map) {
        this.map = map;
        this.inventoryItems = inventoryList;
        list.clear();
        for(InventoryItem item : this.inventoryItems) {
            list.add(item);
        }
        buildMap();
    }

    /**
     * Remove threads from ArrayList
     */
    public void removeThreads() {
        for (Cart cart : carts) {
            if (cart.getThread() != null) {
                cart.getThread().setStop(true);
            }
        }
    }
}

