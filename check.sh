#!/bin/bash

dirs=(/game/server/ /game/server1/)
log=log

get_tel_ip(){
    ssh_user="root"
    ssh_port="22"
    passwd='123456'
    expect -c "
    spawn ssh -l ${ssh_user} -p ${ssh_port}  $1 \"ifconfig eth0|grep -oP '(?<=inet addr:)(\\\d+.){3}\\\d+'\"
    set timeout ${spwan_time}
    expect {
        \"yes/no\"                            {send \"yes\r\";exp_continue}
        \"*password:\"                        {send \"$passwd\r\";exp_continue}
        \"FATAL\"                             {exit 1;exp_continue}
        \"No route to host\"                  {exit 3;exp_continue}
        \"Connection Refused\"                {exit 4;exp_continue}
        \"Connection refused\"                {exit 5;exp_continue}
        \"Host key verification failed\"      {exit 6;exp_continue}
        \"Illegal host key\"                  {exit 7;exp_continue}
        \"Connection Timed Out\"              {exit 8;exp_continue}
        \"Connection timed out\"              {exit 9;exp_continue}
        \"Interrupted system call\"           {exit 10;exp_continue}
        \"Disconnected; connection lost\"     {exit 11;exp_continue}
        \"Authentication failed\"             {exit 12;exp_continue}
        \"File exists\"                       {exit 13;exp_continue}
        \" Error: \"                          {exit 14;exp_continue}
        \" ERROR: \"                          {exit 15;exp_continue}
        \" error \"                           {exit 16;exp_continue}
        \"No such file\"                      {exit 17;exp_continue}
        \"Permission denied,\"                {exit 18;exp_continue}
        \"Destination Unreachable\"           {exit 19;exp_continue}
        \"No such file or directory\"         {exit 20;exp_continue}
        \"rsync error\"                       {exit 21}
    }
    "
}

for m in ${dirs[*]};
do
    if [ -f ${m}config.txt ]; then
        :>${log}
        master_site=`grep Site ${m}config.txt  | awk -F"\"" '{print $2}'`
        master_site_name=${master_site%_*}
        master_site_id=`echo ${master_site##*_} | sed -r 's/^0+([1-9])/\1/g'`
        array=($(grep "site='" ${m}/webapp/client/config.xml | awk -F"'" '{print $2}' | sort -n | xargs))
        for n in ${array[*]}
        do
            echo ${n} >> ${log}
        done
        
        master=""
        slave=""
        second=""
        
        all_site=(`awk -F"_" '{print $1}' ${log}| sort -n | uniq`)
        for i in ${all_site[*]}
        do
            id=(`grep ${i} ${log} | awk -F"_" '{print $2}' | sed -r 's/^0+([1-9])/\1/g' | sort -n | xargs`)
            output=""
            for j in `seq 1 ${#id[@]}`
            do
                begin=${id[$(($j-1))]}
                if [ 1 == $((${id[$j]}-$begin)) ];then
                    output="$output,$begin~${id[$j]}"
                else
                    output="$output,$begin"
                fi
            done
            output=`echo ${output/,/}|sed -r 's/([0-9]+),\1~//g;s/([^0-9])([0-9]+),\2/\1\2/g'`
            if [ "$i" == "$master_site_name" ]; then
                master="$i=$master_site_id-$output"
            else
                slave="$slave$i=$output;"
            fi
        done
        
        if [ -n "$slave" ]; then
            second=`echo $slave | sed 's/;$//' | sed 's/^/;/'`
        fi
	
        m_str=`echo $master | grep "~"`
        if [ -z "$m_str" ]; then
            master=${master%-*}
        fi

        # mysql里site字段生成
        string="$master$second"

        # mysql里ip字段生成
        telecom_ip=`ifconfig eth0 | grep "inet addr" | awk -F":" '{print $2}' | awk '{print $1}'`
        unicom_ip=`ifconfig eth0:1 | grep "inet addr" | awk -F":" '{print $2}' | awk '{print $1}'`
        if [ -z "$unicom_ip" ]; then
            unicom_ip=`ifconfig eth0:0 | grep "inet addr" | awk -F":" '{print $2}' | awk '{print $1}'`
        fi

        # mysql里port字段生成
        port=`echo $m | grep -P -o "\d+"`
        if [ -z "$port" ]; then
            port=0
        fi

        # mysql里resoure字段生成
        line=`grep cdn ${m}/webapp/client/config.xml | wc -l`
        if [ $line == 0 ]; then
            resoure=0
        else
            resoure=1
        fi

        rm $log -rf

        db1_nw_ip=`grep db_strategy ${m}app/config/global_config.xml | awk -F"\"" '{print $6}'`
        db2_nw_ip=`grep db_log ${m}app/config/global_config.xml | awk -F"\"" '{print $6}'`

        db1_ip=`get_tel_ip ${db1_nw_ip} | tail -n 1 | tr -d '\r'`
        db2_ip=`get_tel_ip ${db2_nw_ip} | tail -n 1 | tr -d '\r'`

        # 插入mysql
        echo "call sq.check_insert('$string', '$telecom_ip', '$unicom_ip', '$port', '$resoure', '$db1_ip', '$db2_ip')" | mysql -h192.168.1.1 -P3306 -uroot -p123456
        echo "${master_site} insert data success"

    fi
done
