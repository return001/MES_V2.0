package com.jimi.mes_server.entity.vo;

/**图片信息VO
 * @author   HCJ
 * @date     2019年10月24日 下午1:53:48
 */
public class PictureVO {

	private String pictureNumber;

	private String pictureName;

	private String picturePath;


	public String getPictureNumber() {
		return pictureNumber;
	}


	public void setPictureNumber(String pictureNumber) {
		this.pictureNumber = pictureNumber;
	}


	public String getPictureName() {
		return pictureName;
	}


	public void setPictureName(String pictureName) {
		this.pictureName = pictureName;
	}


	public String getPicturePath() {
		return picturePath;
	}


	public void setPicturePath(String picturePath) {
		this.picturePath = picturePath;
	}
}
