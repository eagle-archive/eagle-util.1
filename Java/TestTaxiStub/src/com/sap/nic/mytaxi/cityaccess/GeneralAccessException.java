package com.sap.nic.mytaxi.cityaccess;

/**
 * This exception is thrown when a device ID is not found
 */
public class GeneralAccessException extends RuntimeException {
	private static final long serialVersionUID = -4614109358656973236L;

	public GeneralAccessException()
    {
        super();
    }

    public GeneralAccessException(String description)
    {
        super(description);
    }
}
