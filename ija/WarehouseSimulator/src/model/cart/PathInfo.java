package model.cart;

import java.util.ArrayList;

/**
 * Class PathInfo
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class PathInfo {
    private ArrayList<Integer> path = new ArrayList<>();

    public ArrayList<Integer> getPath() {
        return path;
    }

    public void setPath(ArrayList<Integer> path) {
        this.path = new ArrayList<Integer>(path);
    }

    public Integer getLastPath () {
        if(this.path.size() < 0) return null;
        return this.path.get(this.path.size()-1);
    }

    public void pushPosition (int lastPosition) {
        this.path.add(lastPosition);
    }



}
