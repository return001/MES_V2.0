package com.jimi.mes_server.entity;

public class SopSQL {

	public static final String SELECT_FACTORY_ID_BY_COLUMN = "SELECT id FROM sop_factory WHERE factory_alias = ? UNION ALL SELECT id FROM sop_factory WHERE abbreviation = ? UNION ALL SELECT id FROM sop_factory WHERE full_name = ?";

	public static final String SELECT_FACTORY = "SELECT id,factory_alias as factoryAlias,abbreviation,full_name as fullName from sop_factory ";

	public static final String SELECT_WORKSHOP_BY_FACTORY = "SELECT * FROM sop_workshop WHERE factory_id = ?";
	
	public static final String SELECT_CUSTOMER_BY_FACTORY = "SELECT * FROM sop_customer WHERE factory_id = ?";
	
	public static final String SELECT_LINE_BY_FACTORY = "SELECT * FROM line WHERE factory_id = ?";
	
	public static final String SELECT_WORKSHOP_BY_COLUMN = "SELECT id FROM sop_workshop WHERE workshop_name = ? UNION ALL SELECT id FROM sop_workshop WHERE workshop_number = ?";

	public static final String SELECT_LINE_BY_WORKSHOP = "SELECT * FROM line WHERE workshop_id = ?";

	public static final String SELECT_WORKSHOP_AND_FACTORY_ABBREVIATION = "SELECT w.id,w.workshop_name as workshopName,w.workshop_number as workshopNumber,f.abbreviation FROM sop_workshop w, sop_factory f WHERE w.factory_id = f.id ";

	public static final String SELECT_SITE_BY_LINE = "SELECT * FROM sop_site WHERE line_id = ?";
	
	
	
	
}
