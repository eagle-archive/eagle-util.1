package com.sap.nic.mytaxi.cityaccess;

/**
 * Defines feature IDs and constant values. Used by IAccessEntry.
 * @see IAccessEntry.getFeatureInt, IAccessEntry.getFeatureFloat, IAccessEntry.getFeatureString  
 */
public interface IAccessFeature {
	/**
	 *  Feature ID for the simple name of the vendor in city level.
	 *  Used by: {@link #getFeatureString(int)}
	 *  Return 0: disabled, 1: enabled
	 */
    static final int FID_VENDOR_SIMPLE_NAME = 1;

    /**
	 *  Feature ID indicating whether price increase is supported.
	 *  Used by: {@link #getFeatureInt(int)}
	 *  Return 0: disabled, 1: enabled
	 */
    static final int FID_PRICE_INCREASE_ENABLED = 2;

	/**
	 *  Feature ID: price increase mode 
	 *  Used by: {@link #getFeatureInt(int)}
	 *  Return PRICE_MODE_FIXED_INCREASE, PRICE_MODE_NEGOTIABLE_ANY, ...
	 */
    static final int FID_PRICE_INCREASE_MODE = 3;
	/** Price increase: support fixed price increase with (1 to n levels) */
    static final int PRICE_MODE_FIXED_INCREASE = 1;
	/** Price increase: price increase can be any number */
    static final int PRICE_MODE_NEGOTIABLE_ANY = 2;

	/**
	 * Feature ID: number of price increase levels
	 *  Used by: {@link #getFeatureInt(int)}
	 * Return number of the fixed price increase levels
	 */
    static final int FID_PRICE_FIXED_INCREASE_LEVEL_NUM = 4;

	/**
	 * Feature ID: price increase levels 
	 *  Used by: {@link #getFeatureFloat(int)}
	 * Return the fixed price increase for the specified level
	 */
    static final int FID_PRICE_FIXED_INCREASE_LEVEL1 = 11;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL2 = 12;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL3 = 13;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL4 = 14;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL5 = 15;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL6 = 16;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL7 = 17;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL8 = 18;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL9 = 19;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL10 = 20;

	/**
	 * Feature ID: indicates whether enlarge search is supported
	 *  Used by: {@link #getFeatureInt(int)}
	 * Return 0: disabled, 1: enabled
	 */
    static final int FID_ENLARGE_SEARCH_ENABLED = 30;

 	/**
 	 * Feature ID: indicates whether the vendor can also return a list of taxis near by.
 	 * If enabled, IAccessEntry.getTaxisNearBy and IAccessEntry.trackTaxiWithNearBy are supported
	 *  Used by: {@link #getFeatureInt(int)}
 	 * Return 0: disabled, 1: enabled
 	 */
    static final int FID_GET_NEARBY_TAXIS_ENABLED = 31;

    /**
 	 * Feature ID: indicates whether the urge taxi driver is supported.
	 *  Used by: {@link #getFeatureInt(int)}
 	 * Return 0: disabled, 1: enabled
 	 */
    static final int FID_URGE_TAXI_ENABLED = 32;

    /**
     * Returns the feature value in integer type supported by the city level taxi calling provider.
     *
     * @param feature ID.
     * @return the feature in integer. If the feature is something like enable/disable, 1 means enable,
     * 0 means disable.
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public int getFeatureInt(int featureId) throws IllegalArgumentException;

	/**
     * Returns the feature value in float type supported by the city level taxi calling provider.
     *
     * @param feature ID.
     * @return the feature in float/double.
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public double getFeatureFloat(int featureId) throws IllegalArgumentException;

    /**
     * Returns the feature string supported by the city level taxi calling provider.
     *
     * @param feature ID.
     * @return the feature in string.
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public String getFeatureString(int featureId) throws IllegalArgumentException;
}
