package com.sap.nic.mytaxi.cityaccess;

/**
 * Defines feature IDs and values. Used by IAccessManager.
 * @see IAccessManager.getFeature(int)
 */
public class Feature {
	/** Feature ID indicating whether price increase is supported */
    static final int FID_PRICE_INCREASE_ENABLED = 1;
    
	/** Feature ID: price increase mode */
    static final int FID_PRICE_INCREASE_MODE = 2;

	/** Feature ID: number of price increase levels */
    static final int FID_PRICE_FIXED_INCREASE_LEVEL_NUM = 3;

	/** Feature ID: price increase levels */
    static final int FID_PRICE_FIXED_INCREASE_LEVEL1 = 4;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL2 = 5;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL3 = 6;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL4 = 7;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL5 = 8;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL6 = 9;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL7 = 10;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL8 = 11;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL9 = 12;
    static final int FID_PRICE_FIXED_INCREASE_LEVEL10 = 13;

	/** Price increase: price increase is forbidden */
    static final int PRICE_MODE_INCREASE_FORBIDDEN = 0;
	/** Price increase: support fixed price increase with (1 to n levels) */
    static final int PRICE_MODE_FIXED_INCREASE = 1;
	/** Price increase: price increase can be any number */
    static final int PRICE_MODE_ANY_INCREASE = 2;

	/** Feature ID: whether enlarge search is supported */
    static final int FID_ENLARGE_SEARCH_ENABLED = 14;
    
}
