!!enter XTP!!
	!!travel fields!!
		!!if valid_name("usage") && !strcmp(@usage,"DownTable")!!
			!!let InsertTime="no"!!
			!!travel self!!
				!!if !strcmp(@name,"InsertTime")!!
					!!let InsertTime="yes"!!
				!!endif!!
			!!next!!
			!!if !strcmp(@InsertTime,"yes")!!
call create_partition_by_month('perpetual','t_D_!!@name!!')
			!!endif!!
		!!endif!!
	!!next!!
!!leave!!