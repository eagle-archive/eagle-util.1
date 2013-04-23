package com.sap.nic.mytaxi.cityaccess;

/**
 * This exception is thrown when a device ID is not found
 */
public class GeneralAccessException extends RuntimeException {
    public GeneralAccessException()
    {
        super();
    }

    public GeneralAccessException(String description)
    {
        super(description);
    }
}
