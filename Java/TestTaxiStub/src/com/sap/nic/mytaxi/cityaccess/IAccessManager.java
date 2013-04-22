package com.sap.nic.mytaxi.cityaccess;

/**
 * This interface gives access to city level taxi calling provider (e.g., Nanjing Tong Yong,
 * Shanghai Da Zhong), who is supposed to implement this interface in Java.
 */
public interface IAccessManager {
    /**
     * Returns the feature value supported by the city level taxi calling provider.
     *
     * @param feature ID.
     * @return the feature in integer. If the feature is something like enable/disable, convert
     * the return value to boolean
     * @throws IllegalArgumentException if the feature ID does not exist
     * @see Feature
     */
	public int getFeatureValue(int featureId) throws IllegalArgumentException;

    /**
     * Returns the feature string supported by the city level taxi calling provider.
     * NOTE: no feature IDs return string for the time being. This API is created for future use 
     * 
     * @param feature ID.
     * @return the feature in integer. If the feature is something like enable/disable, convert
     * the return value to boolean
     * @throws IllegalArgumentException if the feature ID does not exist
     * @see Feature
     */
	public String getFeatureString(int featureId) throws IllegalArgumentException;	
}
