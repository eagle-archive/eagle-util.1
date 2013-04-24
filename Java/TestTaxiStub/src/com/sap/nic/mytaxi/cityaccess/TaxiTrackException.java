package com.sap.nic.mytaxi.cityaccess;

/**
 * This exception is thrown when taxi tracking error happens
 */
public class TaxiTrackException extends RuntimeException {
	/**
	 * Error No: device ID is not found<br>
	 * description should contain the device ID.<br>
	 * 错误代码：设备标志没有找到。description参数中应包含出错的设备标志。
	 */
	public static final int ERR_NO_DEVICE_NOT_FOUND = 200;

	/**
	 * Error No: function not supported<br>
	 * description should contain what function is not supported.<br>
	 * 错误代码：功能不支持。description参数中应说明具体什么功能不支持。
	 */
	public static final int ERR_NO_FUNCTION_NOT_SUPPORTED = 201;


	private static final long serialVersionUID = -8855254829945924195L;
	protected long errorCode;

	/**
	 * Constructor
	 * 
	 * @param errorCode error code for taxi tracking
	 */
	public TaxiTrackException(long errorCode) {
        super();
        this.errorCode = errorCode;
    }

	/**
	 * Constructor
	 * 
	 * @param errorCode error code for taxi tracking
	 * @param description error description
	 */
    public TaxiTrackException(long errorCode, String description) {
        super(description);
        this.errorCode = errorCode;
    }

    /**   
     * @return the errorCode   
     */   
    public long getErrorCode() {    
        return errorCode;    
    }
}
