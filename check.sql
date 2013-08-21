/* 独立信息 */
create table indepe_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    site text,
    telecom_ip varchar(20) not null default '',
    unicom_ip varchar(20) not null default '',
    port integer not null default 0,        -- 0是从5000开始,1是从5004开始
    resoure integer not null default 0,     -- 0是res本地资源域名,1是cdn域名
    db1_ip varchar(20) not null default '',
    db2_ip varchar(20) not null default ''
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 共同信息 */
create table common_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    site_name text,
    domain text
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* cdn信息 */
create table cdn_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    telecom_ip varchar(100) not null default '',
    unicom_ip varchar(100) not null default ''
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* port信息 */
create table port_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    base_port integer not null default 0,
    time_port integer not null default 0
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 名字信息 */
create table name_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    site_name text,
    cn_name text
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 插入各区数据存储过程 */
delimiter //
drop procedure if exists check_insert//
create procedure check_insert(
    in i_site text,
    in i_telecom_ip varchar(20),
    in i_unicom_ip varchar(20),
    in i_port integer,
    in i_resoure integer,
    in i_db1_ip varchar(20),
    in i_db2_ip varchar(20)
)
begin
    insert into sq.indepe_info(site, telecom_ip, unicom_ip, port, resoure, db1_ip, db2_ip) values(i_site, i_telecom_ip, i_unicom_ip, i_port, i_resoure, i_db1_ip, i_db2_ip);
end//
delimiter ;
