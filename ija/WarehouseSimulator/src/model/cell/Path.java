package model.cell;

/**
 * Class Path
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class Path extends Cell{
    private boolean closure = false;

    @Override
    public void setClosure(boolean closure) {
        this.closure = closure;
    }
    @Override
    public boolean isClosed() {
        return closure;
    }
}
