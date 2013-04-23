package com.sap.nic.mytaxi.cityaccess;

/**
 * Defines feature IDs, constant values and methods about access features.
 *
 * 移动招车接入提供商支持的特性接口。该接口应由接入提供商实现。
 */
public interface IAccessFeature {
	/**
	 *  Feature ID for the simple name of the vendor in city level.
	 *  提供商简单名称
	 *  Used by: {@link #getFeatureString(int)}
	 */
	public static final int FID_VENDOR_SIMPLE_NAME = 1;

    /**
	 *  Feature ID indicating whether price increase is supported.
	 *  是否允许加价
	 *  Used by: {@link #getFeatureInt(int)}
	 *  Return 0: disabled, 1: enabled
	 */
	public static final int FID_PRICE_INCREASE_ENABLED = 2;

	/**
	 *  Feature ID: price increase mode 
	 *  加价模式
	 *  Used by: {@link #getFeatureInt(int)}
	 *  Return PRICE_MODE_FIXED_INCREASE, PRICE_MODE_NEGOTIABLE_ANY, ...
	 */
	public static final int FID_PRICE_INCREASE_MODE = 3;
	/**
	 * Price increase mode: support fixed price increase with (1 to n levels)
	 * 加价模式：允许固定加价，可以有一个或者多个固定价格
	 */
	public static final int PRICE_MODE_FIXED_INCREASE = 1;
	/**
	 * Price increase mode: price increase can be any number
	 * 加价模式：无固定加价，加价不限
	 */
	public static final int PRICE_MODE_ANY_INCREASE = 2;

	/**
	 * Feature ID: number of price increase levels
	 * Used by: {@link #getFeatureInt(int)}
	 * Return number of the fixed price increase levels
	 * 对于固定加价：固定价格的个数
	 */
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL_NUM = 4;

	/**
	 * Feature ID: price increase levels 
	 *  Used by: {@link #getFeatureFloat(int)}
	 * Return the fixed price increase for the specified level
	 * 对应于某个固定加价，具体的价格
	 */
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL1 = 11;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL2 = 12;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL3 = 13;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL4 = 14;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL5 = 15;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL6 = 16;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL7 = 17;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL8 = 18;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL9 = 19;
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL10 = 20;

	/**
	 * Feature ID: indicates whether enlarge search is supported
	 *  Used by: {@link #getFeatureInt(int)}
	 * Return 0: disabled, 1: enabled
	 * 是否允许扩大招车范围
	 */
	public static final int FID_ENLARGE_SEARCH_ENABLED = 30;

 	/**
 	 * Feature ID: indicates whether the vendor can also return a list of taxis near by.
 	 * If enabled, IAccessEntry.getTaxisNearBy and IAccessEntry.trackTaxiWithNearBy are supported
	 *  Used by: {@link #getFeatureInt(int)}
 	 * Return 0: disabled, 1: enabled
 	 * 是否允许搜索附近的出租车
 	 */
	public static final int FID_GET_NEARBY_TAXIS_ENABLED = 31;

    /**
 	 * Feature ID: indicates whether the urge taxi driver is supported.
	 *  Used by: {@link #getFeatureInt(int)}
 	 * Return 0: disabled, 1: enabled
 	 * 是否允许催车
 	 */
	public static final int FID_URGE_TAXI_ENABLED = 32;

    /**
     * Returns the feature value in integer type supported by the city level taxi calling provider.
     *
     * @param featureId feature ID.
     * @return the feature in integer. 
     *         If the feature is something like enable/disable, 1 - means enable, 0 - means disable.
     *         返回值为整形
     *         
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public int getFeatureInt(int featureId) throws IllegalArgumentException;

	/**
     * Returns the feature value in float type supported by the city level taxi calling provider.
     *
     * @param featureId feature ID.
     * @return the feature in float/double.
     *         返回值为浮点数
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public double getFeatureFloat(int featureId) throws IllegalArgumentException;

    /**
     * Returns the feature string supported by the city level taxi calling provider.
     *
     * @param featureId feature ID.
     * @return the feature in string.
     *         返回值为字符串
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public String getFeatureString(int featureId) throws IllegalArgumentException;
}
