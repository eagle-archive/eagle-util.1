package com.sap.nic.mytaxi.cityaccess;

/**
 * Defines feature IDs, constant values and methods about access features.
 *
 * �ƶ��г������ṩ��֧�ֵ����Խӿڡ��ýӿ�Ӧ�ɽ����ṩ��ʵ�֡�
 */
public interface IAccessFeature {
	/**
	 *  Feature ID for the simple name of the vendor in city level.
	 *  �ṩ�̼�����
	 *  Used by: {@link #getFeatureString(int)}
	 */
	public static final int FID_VENDOR_SIMPLE_NAME = 1;

    /**
	 *  Feature ID indicating whether price increase is supported.
	 *  �Ƿ�����Ӽ�
	 *  Used by: {@link #getFeatureInt(int)}
	 *  Return 0: disabled, 1: enabled
	 */
	public static final int FID_PRICE_INCREASE_ENABLED = 2;

	/**
	 *  Feature ID: price increase mode 
	 *  �Ӽ�ģʽ
	 *  Used by: {@link #getFeatureInt(int)}
	 *  Return PRICE_MODE_FIXED_INCREASE, PRICE_MODE_NEGOTIABLE_ANY, ...
	 */
	public static final int FID_PRICE_INCREASE_MODE = 3;
	/**
	 * Price increase mode: support fixed price increase with (1 to n levels)
	 * �Ӽ�ģʽ������̶��Ӽۣ�������һ�����߶���̶��۸�
	 */
	public static final int PRICE_MODE_FIXED_INCREASE = 1;
	/**
	 * Price increase mode: price increase can be any number
	 * �Ӽ�ģʽ���޹̶��Ӽۣ��Ӽ۲���
	 */
	public static final int PRICE_MODE_ANY_INCREASE = 2;

	/**
	 * Feature ID: number of price increase levels
	 * Used by: {@link #getFeatureInt(int)}
	 * Return number of the fixed price increase levels
	 * ���ڹ̶��Ӽۣ��̶��۸�ĸ���
	 */
	public static final int FID_PRICE_FIXED_INCREASE_LEVEL_NUM = 4;

	/**
	 * Feature ID: price increase levels 
	 *  Used by: {@link #getFeatureFloat(int)}
	 * Return the fixed price increase for the specified level
	 * ��Ӧ��ĳ���̶��Ӽۣ�����ļ۸�
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
	 * �Ƿ����������г���Χ
	 */
	public static final int FID_ENLARGE_SEARCH_ENABLED = 30;

 	/**
 	 * Feature ID: indicates whether the vendor can also return a list of taxis near by.
 	 * If enabled, IAccessEntry.getTaxisNearBy and IAccessEntry.trackTaxiWithNearBy are supported
	 *  Used by: {@link #getFeatureInt(int)}
 	 * Return 0: disabled, 1: enabled
 	 * �Ƿ��������������ĳ��⳵
 	 */
	public static final int FID_GET_NEARBY_TAXIS_ENABLED = 31;

    /**
 	 * Feature ID: indicates whether the urge taxi driver is supported.
	 *  Used by: {@link #getFeatureInt(int)}
 	 * Return 0: disabled, 1: enabled
 	 * �Ƿ�����߳�
 	 */
	public static final int FID_URGE_TAXI_ENABLED = 32;

    /**
     * Returns the feature value in integer type supported by the city level taxi calling provider.
     *
     * @param featureId feature ID.
     * @return the feature in integer. 
     *         If the feature is something like enable/disable, 1 - means enable, 0 - means disable.
     *         ����ֵΪ����
     *         
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public int getFeatureInt(int featureId) throws IllegalArgumentException;

	/**
     * Returns the feature value in float type supported by the city level taxi calling provider.
     *
     * @param featureId feature ID.
     * @return the feature in float/double.
     *         ����ֵΪ������
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public double getFeatureFloat(int featureId) throws IllegalArgumentException;

    /**
     * Returns the feature string supported by the city level taxi calling provider.
     *
     * @param featureId feature ID.
     * @return the feature in string.
     *         ����ֵΪ�ַ���
     * @throws IllegalArgumentException if the feature ID is not appropriate for this method
     */
	public String getFeatureString(int featureId) throws IllegalArgumentException;
}
