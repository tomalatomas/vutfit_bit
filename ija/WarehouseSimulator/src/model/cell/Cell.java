package model.cell;
/**
 * Class Cell
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class Cell {
    private boolean closure = false;

    public void setClosure(boolean closure) {
        this.closure = closure;
    }

    public boolean isClosed() {
        return closure;
    }
}
