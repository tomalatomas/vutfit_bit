package model.cart;

import main.CartThread;

import java.util.ArrayList;
import java.util.Objects;

/**
 * Class Cart
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class Cart {
    final int CART_SIZE = 100;

    Request request = null;
    int size = CART_SIZE;
    boolean isFree = true;
    int posX;
    int posY;
    ArrayList<Integer> remainingPath = new ArrayList<>();
    CartThread thread = null;

    public Cart(int position) {
        this.posX = position % 16;
        this.posY = position / 16;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Cart cart = (Cart) o;
        return isFree == cart.isFree && posX == cart.posX && posY == cart.posY && Objects.equals(request, cart.request) && Objects.equals(remainingPath, cart.remainingPath);
    }


    public ArrayList<Integer> getRemainingPath() {
        return remainingPath;
    }

    public void setRemainingPath(ArrayList<Integer> remainingPath) {
        this.remainingPath = remainingPath;
    }

    public void updateRemainingPath() {
        this.remainingPath.remove(0);
    }

    public int getPosX() {
        return posX;
    }

    public void setPosX(int posX) {
        this.posX = posX;
    }

    public int getPosY() {
        return posY;
    }

    public void setPosY(int posY) {
        this.posY = posY;
    }

    public Request getRequest() {
        return request;
    }

    public void setRequest(Request request) {
        this.request = request;
    }

    public int getSize() {
        return size;
    }

    public void setSize(int size) {
        this.size = size;
    }

    public boolean isFree() {
        return isFree;
    }

    public void setFree(boolean free) {
        isFree = free;
    }

    public CartThread getThread() {
        return thread;
    }

    public void setThread(CartThread thread) {
        this.thread = thread;
    }


    /**
     * Empty cart
     * remove request, path, set default size
     */
    public void emptyCart() {
        this.request = null;
        this.remainingPath = null;
        this.size = CART_SIZE;
    }



}
