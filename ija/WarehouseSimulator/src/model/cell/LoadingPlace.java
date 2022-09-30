package model.cell;

import java.util.ArrayList;
import java.util.Random;

/**
 * Class LoadingPlace
 * @author Martin Mlýnek (xmlyne06)
 * @author Tomáš Tomala (xtomal02)
 */
public class LoadingPlace extends Cell{
    private static ArrayList<Integer> loadingPlacesPositions = new ArrayList<>();

    public static ArrayList<Integer> getLoadingPlacesPositions() {
        if(loadingPlacesPositions.size() == 0) return null;
        return loadingPlacesPositions;
    }
    public static void createLoadingPlace(int position){
        loadingPlacesPositions.add(position);
    }

    public static void clearLoadingPlaces(){
        loadingPlacesPositions.clear();
    }

    /**
     * Get random loading place
     * @return id of loading place
     */
    public static Integer getRandomLoadPlace() {
        Random rand = new Random();
        int pos = rand.nextInt(loadingPlacesPositions.size());
        return loadingPlacesPositions.get(pos);
    }
}
