package ndr.brt.audio;

import android.os.Build;

import androidx.annotation.RequiresApi;

import java.util.function.BiFunction;

@RequiresApi(api = Build.VERSION_CODES.N)
public interface CoordinateToFrequency extends BiFunction<Float, Integer, Float> {

    float MAXIMUM_FREQUENCY = 10000f;

    static CoordinateToFrequency coordinateToFrequency() {
        return (coordinate, total) -> MAXIMUM_FREQUENCY - (float) ((MAXIMUM_FREQUENCY * Math.log10(coordinate + 1))/Math.log10(total + 1));
    }
}
