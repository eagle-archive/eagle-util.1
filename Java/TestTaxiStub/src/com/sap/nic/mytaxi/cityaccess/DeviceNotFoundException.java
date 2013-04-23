package com.sap.nic.mytaxi.cityaccess;

/**
 * This exception is thrown when a device ID is not found
 */
public class DeviceNotFoundException extends RuntimeException {
	private static final long serialVersionUID = 4216114802281581340L;

	public DeviceNotFoundException()
    {
        super();
    }

    public DeviceNotFoundException(String devId)
    {
        super("Taxi: " + devId + " is not found");
    }
}
