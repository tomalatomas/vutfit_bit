package main;

import javafx.application.Platform;
import javafx.scene.paint.Color;
import model.cart.Cart;
import model.cart.PathInfo;
import model.cart.Request;
import model.cart.RequestItem;
import model.cell.Cell;
import model.cell.LoadingPlace;
import model.cell.Shelf;
import model.item.InventoryItem;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


public class CartThread extends Thread {


    private Controller controller;
    private Cart cart;
    private Cell[][] map = Controller.map;
    private boolean stop = false;

    public CartThread(Controller controller, Cart cart) {
        this.controller = controller;
        this.cart = cart;
    }
    /**
     * Checks if cell in map is obstacle
     * @param cell cell to check
     */
    public boolean isObstacle(Cell cell){
        if (cell instanceof Shelf){
            return true;
        }
        return cell.isClosed();
    }

    public void run()
    {
        // Only one thread can send a message
        // at a time.
        this.executeCart();
        this.interrupt();

    }
    /**
     * Getting position, where we can load from shelf
     * @param shelfPos - position of shelf to load
     * @return list - list with loading places for shelf
     */
    private ArrayList<Integer> getLoadingPlaces(int shelfPos) {
        ArrayList<Integer> list = new ArrayList<>();
        int posX = shelfPos  % 16;
        int posY = shelfPos / 16;

        int leftX = posX-1;
        int leftY = posY;
        if( (leftX >= 0 && leftX < 16) && !isObstacle(map[leftY][leftX])){
            //Left neighbour
            int neigbPos = (leftX)+(leftY*16);
            list.add(neigbPos);
        }
        int rightX = posX+1;
        int rightY = posY;
        if( (rightX >= 0 && rightX < 16) && !isObstacle(map[rightY][rightX])){
            //Right neighbour
            int neigbPos = (rightX)+(rightY*16);
            list.add(neigbPos);
        }
        int upX = posX;
        int upY = posY-1;
        if( (upY >= 0 && upY < 16) && !isObstacle(map[upY][upX])){
            //Up neighbour
            int neigbPos = upX+(upY*16);
            list.add(neigbPos);
        }
        int downX = posX;
        int downY = posY+1;
        if( (posY+1 >= 0 && posY+1 < 16) && !isObstacle(map[downY][downX])){
            //Down neighbour
            int neigbPos = downX+(downY*16);
            list.add(neigbPos);
        }
        return list;
    }
    /**
     * Send cart for every requested item
     */
    public void executeCart(){
        int startPos = cart.getPosX()+(cart.getPosY()*16);
        PathInfo path = null;

        for(RequestItem requestedItem : cart.getRequest().getRequestedList()){
            if (stop) return;
            if(requestedItem.getRequestedQuantity() == 0) continue;
            int shelfPos = requestedItem.getShelfPosition();
            ArrayList<Integer> shelfLoadingPlaces = getLoadingPlaces(shelfPos);
            for(int shelfloadpos : shelfLoadingPlaces) {
                AStar as = new AStar(getObstacleMatrix(), startPos);
                path = as.findPathTo(shelfloadpos);
                if(path != null){
                    break;
                }

            }
            if (path == null){
                continue;
            } else {
                if(!stop) {
                    //printPath(path);
                    moveCart(path.getPath());
                    int shelfX = shelfPos % 16;
                    int shelfY = shelfPos / 16;
                    if(!stop) {
                        cartIsOnLoadingPlace((Shelf) map[shelfY][shelfX], cart);
                    }
                }
            }
            startPos = cart.getPosX()+(cart.getPosY()*16);
        }
        if(!stop){
            AStar as = new AStar(getObstacleMatrix(), cart.getPosX()+(cart.getPosY()*16));
            path = as.findPathTo(LoadingPlace.getRandomLoadPlace());
            while(path == null){
                try {
                    this.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                as = new AStar(getObstacleMatrix(), cart.getPosX()+(cart.getPosY()*16));
                path = as.findPathTo(LoadingPlace.getRandomLoadPlace());
            }
            moveCart(path.getPath());
            returnToStock(cart);
        }
        if(!stop) {
            Platform.runLater(new Runnable(){
                @Override
                public void run() {
                    controller.endWindows(cart);
                }
            });
        }

    }
    /**
     * Cart returns to stock unfilled requests
     * @param cart - cart sent for request
     */
    private void returnToStock(Cart cart) {
        Request req = cart.getRequest();
        for(RequestItem item : req.getRequestedList()){
            int remainingQuantity = item.getRequestedQuantity();
            String name = item.getName();
            for(InventoryItem inventoryItem : controller.inventoryItems){
                if(inventoryItem.getName() == item.getName()){
                    inventoryItem.addInStock(remainingQuantity);
                }
            }
        }
        //RebuildingTableView
        controller.list.clear();
        for(InventoryItem item : controller.inventoryItems) {
            controller.list.add(item);
        }
    }

    public void printPath(PathInfo path ){
        ArrayList<Integer> pos = path.getPath();
        System.out.print("[");
        for(int i : pos){
            System.out.print("->"+i);
        }
        System.out.print("]\n");

    }
    /**
     * Cart is on loading place a loading can begin
     * @param shelf - shelf to load from
     * @param cart - cart to load to
     */
    public void cartIsOnLoadingPlace(Shelf shelf, Cart cart) {
        Request request = cart.getRequest();
        for (RequestItem item: request.getRequestedList()) {
            // jednotlive requestItemy
            String name = item.getName();
            if (shelf.containsItem(name) && item.getRequestedQuantity() != 0) {
                int newQuantity = shelf.removeItemFromShelf(name, item.getRequestedQuantity());
                int oldQuantity = item.getRequestedQuantity();
                item.setRequestedQuantity(newQuantity);
                item.setInVehicle(oldQuantity - newQuantity);
            }
        }
        //request.printRequestVehicle();
        //shelf.printShelf();
    }


    /**
     * Moving cart on map
     * @param path - list with path
     */
    public void moveCart(ArrayList<Integer> path){
        if (stop) return;
        ArrayList<Integer> remainingPath = new ArrayList<>(path) ;
        cart.setRemainingPath(remainingPath);
        for (int pos : path) {
            if (stop) return;
            int posX = pos % 16;
            int posY = pos / 16;
            cart.setPosX(posX);
            cart.setPosY(posY);
            controller.gMap[posY][posX].setStroke(Color.BLACK);
            controller.gMap[posY][posX].setStrokeWidth(1);

            cart.updateRemainingPath();
            synchronized (controller) {
                controller.refreshMap();
            }
            try {
                this.sleep(2000 / Controller.simulationSpeed*2);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }

    }


    /**
     * Returns matrix filled with 0 and 1
     * 1 symbolizes path, 0 is obstacle
     * @return matrix - obstacle matrix
     */
    private Integer[][] getObstacleMatrix() {
        Integer[][] matrix = new Integer[16][16];
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x< 16; x++) {
                if(isObstacle(map[y][x])){
                    matrix[y][x] = -1;
                } else {
                    matrix[y][x] = 0;
                }
            }
        }
        return matrix;
    }

    public void setStop(boolean stop) {
        this.stop = stop;
    }
}
/**
 * Class AStar
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
class AStar {
    private final List<Node> open;
    private final List<Node> closed;
    private final List<Node> path;
    private final Integer[][] maze;
    private Node now;
    private final int xstart;
    private final int ystart;
    private int xend, yend;
    private final boolean diag;

    // Node class for convienience
    static class Node implements Comparable {
        public Node parent;
        public int x, y;
        public double g;
        public double h;
        Node(Node parent, int xpos, int ypos, double g, double h) {
            this.parent = parent;
            this.x = xpos;
            this.y = ypos;
            this.g = g;
            this.h = h;
        }
        // Compare by f value (g + h)
        @Override
        public int compareTo(Object o) {
            Node that = (Node) o;
            return (int)((this.g + this.h) - (that.g + that.h));
        }
    }

    AStar(Integer[][] maze, int start) {
        this.open = new ArrayList<>();
        this.closed = new ArrayList<>();
        this.path = new ArrayList<>();
        this.maze = maze;
        this.now = new Node(null, start%16, start/16, 0, 0);
        this.xstart = start%16;
        this.ystart = start/16;
        this.diag = false;
    }
    /**
     * Finds path to xend/yend or returns null
     *
     * @param destination destination of path
     * @return path
     **/
    public PathInfo findPathTo(int destination) {
        this.xend = destination%16;
        this.yend = destination/16;
        this.closed.add(this.now);
        addNeigborsToOpenList();
        while (this.now.x != this.xend || this.now.y != this.yend) {
            if (this.open.isEmpty()) { // Nothing to examine
                return null;
            }
            this.now = this.open.get(0); // get first node (lowest f score)
            this.open.remove(0); // remove it
            this.closed.add(this.now); // and add to the closed
            addNeigborsToOpenList();
        }
        this.path.add(0, this.now);
        while (this.now.x != this.xstart || this.now.y != this.ystart) {
            this.now = this.now.parent;
            this.path.add(0, this.now);
        }
        return convertToPathInfo(this.path);
    }
    /**
     * Converts astar path to sequences of positions in map
     *
     * @param path astar path to convert
     * @return positions sequence
     **/
    private PathInfo convertToPathInfo(List<Node> path) {
        PathInfo pathInfo = new PathInfo();
        for(Node n : path){
            int position = n.x+(n.y*16);
            pathInfo.pushPosition(position);
        }
        return pathInfo;
    }

    /**
     * Looks in a given list for a node
     * @return NeightborInListFound
     **/
    private static boolean findNeighborInList(List<Node> array, Node node) {
        return array.stream().anyMatch((n) -> (n.x == node.x && n.y == node.y));
    }
    /**
     * Calulate distance between this.now and xend/yend
     *
     * @return distance
     **/
    private double distance(int dx, int dy) {
        if (this.diag) { // if diagonal movement is allowed
            return Math.hypot(this.now.x + dx - this.xend, this.now.y + dy - this.yend); // return hypothenuse
        } else {
            return Math.abs(this.now.x + dx - this.xend) + Math.abs(this.now.y + dy - this.yend); // else return "Manhattan distance"
        }
    }
    /**
     * Returns neighbours of cell that are not obstacles
     */
    private void addNeigborsToOpenList() {
        Node node;
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (!this.diag && x != 0 && y != 0) {
                    continue; // skip if diagonal movement is not allowed
                }
                node = new Node(this.now, this.now.x + x, this.now.y + y, this.now.g, this.distance(x, y));
                if ((x != 0 || y != 0) // not this.now
                        && this.now.x + x >= 0 && this.now.x + x < this.maze[0].length // check maze boundaries
                        && this.now.y + y >= 0 && this.now.y + y < this.maze.length // check maze boundaries
                        && this.maze[this.now.y + y][this.now.x + x] != -1 // check if square is walkable
                        && !findNeighborInList(this.open, node) && !findNeighborInList(this.closed, node)) { // if not already done
                    node.g = node.parent.g + 1.; // Horizontal/vertical cost = 1.0
                    node.g += maze[this.now.y + y][this.now.x + x]; // add movement cost for this square
                    this.open.add(node);
                }
            }
        }
        Collections.sort(this.open);
    }

}
