package ndr.brt.audio;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class CoordinateToFrequencyTest {

    private CoordinateToFrequency coordinateToFrequency;

    @Before
    public void setUp() {
        coordinateToFrequency = CoordinateToFrequency.coordinateToFrequency();
    }

    @Test
    public void at_zero_the_frequency_is_at_maximum() {
        assertEquals(Float.valueOf(10000), coordinateToFrequency.apply(0f, 100));
    }

    @Test
    public void at_max_the_frequency_is_zero() {
        assertEquals(Float.valueOf(0), coordinateToFrequency.apply(100f, 100));
    }

    @Test
    public void scale_should_be_logaritmic() {
        assertEquals(Float.valueOf(1480.5566f), coordinateToFrequency.apply(50f, 100));
    }
}