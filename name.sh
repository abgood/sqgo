#!/bin/bash

while read echoline
do
    cn_name=`echo ${echoline} | awk -F":::" '{print $1}'`
    site_name=`echo ${echoline} | awk -F":::" '{print $2}'`
    echo "insert into sq.name_info(site_name,cn_name) values('$site_name','${cn_name}');" | mysql -hlocalhost -P3306 -uroot -p123456
done < ./sq_list
