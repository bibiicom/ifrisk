# -*- coding:UTF-8 -*-
import decimal
import json
import os

import requests

"""
准备工作：
* 安装python/pip：3.8+
* 安装依赖：pip install pymysql requests

运行方式：
* 配置参数：API访问路径，标的列表，周期列表，操作类型
* 生成脚本：python update_lastkline.py > /tmp/update_lastkline.log
* 检查脚本是否生成：tail /tmp/update_lastkline.log
* 执行脚本：sh /tmp/exec_command.sh（见日志/tmp/update_lastkline.log最后一行）
"""

# API访问路径
API_POST_BASE_URL = 'http://127.0.0.1:8090'  # 用于导入数据到最新K线
API_GET_BASE_URL = 'http://127.0.0.1:8070'  # 用于查询K线数据

# 标的列表
# INSTRUMENTID_LIST = ["BTCUSD", "ETHUSD", "XRPUSD",
#                      "BCH/USDT", "BTC/USDT", "DOT/USDT", "EOS/USDT", "ETH/USDT", "FIL/USDT", "KEY/USDT", "LTC/USDT", "PAX/USDT", "TRX/USDT", "USDC/USDT", "XRP/USDT"]
INSTRUMENTID_LIST = ["BTCUSD", "BTC/USDT"]

# 周期列表
PERIODID_LIST = ["1m", "5m", "15m", "30m", "1h", "4h", "12h", "1d", "1w", "1o"]
# PERIODID_LIST = ["1m", "5m", "1h", "4h", "1d", "1w", "1o"]

"""
操作类型：
    clear: 清空所有周期的数据后重新导入
    update: 只更新一部分周期的数据
"""
OPERATE_TYPE = "clear"
# OPERATE_TYPE = "update"

INSERT_COUNT_LIMIT = 1
# UPDATE_COUNT_LIMIT = 90
CLEAR_COMMAND_FORMAT = 'curl -d "{}" -X POST {}/action/v1.0/SendClearLastKLine'.format('{}', API_POST_BASE_URL)
# INSERT_COMMAND_FORMAT = 'curl -d "{}" -X POST {}/action/v1.0/SendInsert/LastKLine'.format('{}', API_POST_BASE_URL)
# UPDATE_COMMAND_FORMAT = 'curl -d "{}" -X POST {}/action/v1.0/SendUpdate/LastKLine'.format('{}', API_POST_BASE_URL)
INSUPD_COMMAND_FORMAT = 'curl -d "{}" -X POST {}/action/v1.0/SendInsUpd/LastKLine'.format('{}', API_POST_BASE_URL)

# 生成的执行脚本路径
EXEC_FILEPATH = "/tmp/exec_command.sh"

EXCHANGEID = "Test"
TIMEZONE = 57600
TOTAL_COUNT = 0


class DecimalEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, decimal.Decimal):
            return float(o)
        super(DecimalEncoder, self).default(o)


def collect_api_his_kline(instrumentId, period):
    url = f"{API_GET_BASE_URL}/query/v1.0/KLine?ExchangeID={EXCHANGEID}&InstrumentID={instrumentId}&Type={period}&Count={INSERT_COUNT_LIMIT}"
    response = requests.get(url)
    if not str(response.status_code).startswith('2'):
        raise Exception(response.text)

    try:
        datas = response.json()
        print(f"collect API klines, ExchangeID:{EXCHANGEID}, InstrumentID:{instrumentId}, PeriodID:{period}, size: {len(datas)}")
        return datas
    except ValueError:
        raise Exception('Invalid Response: {}'.format(response.text))


def write_execute_command_to_file(klines, execute_command_format):
    with open(EXEC_FILEPATH, "a", encoding="utf8") as f:
        for kline in klines:
            kline_json_str = json.dumps(kline, cls=DecimalEncoder)
            kline_json_str = kline_json_str.replace('"', '\\"')
            kline_json_str = kline_json_str.replace("'", '\\"')
            output_str = execute_command_format.format(kline_json_str)
            f.write(output_str + '\n')
            global TOTAL_COUNT
            TOTAL_COUNT += 1


# def collect_execute_commands(klines):
#   # 插入或更新最新的一条K线
#   write_execute_command_to_file([klines[0]], INSUPD_COMMAND_FORMAT)
# 
#   # 插入其他K线，存在则失败
#   insert_klines = klines[1:]
#   write_execute_command_to_file(insert_klines, INSERT_COMMAND_FORMAT)
# 
#   if OPERATE_TYPE == "update":
#       # 更新K线
#       keep_klines = klines[0:UPDATE_COUNT_LIMIT]  # 由大到小（由新到旧）
#       write_execute_command_to_file(keep_klines, UPDATE_COMMAND_FORMAT)


def sort_klines_by_begintime(klines, reverse=True):
    if type(klines) == list:
        klines.sort(key=lambda kline: kline["BeginTime"], reverse=reverse)
    return klines


if __name__ == '__main__':
    # 参数检查
    if len(INSTRUMENTID_LIST) == 0:
        print("INSTRUMENTID_LIST can not be empty")
        exit(-1)
    if len(PERIODID_LIST) == 0:
        print("PERIODID_LIST can not be empty")
        exit(-1)

    print(f"instrument list:{INSTRUMENTID_LIST}, size:{len(INSTRUMENTID_LIST)}")
    print(f"period list:{PERIODID_LIST}, size:{len(PERIODID_LIST)}")

    # 清理已存在脚本
    if os.path.exists(EXEC_FILEPATH):
        os.remove(EXEC_FILEPATH)

    for instrument_id in INSTRUMENTID_LIST:
        for period_id in PERIODID_LIST:
            if OPERATE_TYPE == "clear":
                # 清理内存K线
                delete_kline = {"ExchangeID": EXCHANGEID, "InstrumentID": instrument_id, "PeriodID": period_id, "TimeZone": TIMEZONE}
                write_execute_command_to_file([delete_kline], CLEAR_COMMAND_FORMAT)
                
            klines = collect_api_his_kline(instrument_id, period_id)
            if len(klines) == 0:
                print(f"skip empty kline, ExchangeID:{EXCHANGEID}, InstrumentID:{instrument_id}, PeriodID:{period_id}, TimeZone:{TIMEZONE}")
                continue

            # 对K线进行时间排序：由大到小（由新到旧）
            # klines = sort_klines_by_begintime(klines)

            # 生成执行脚本
            # collect_execute_commands(klines)
            
            # 插入或更新最新的一条K线
            write_execute_command_to_file([klines[0]], INSUPD_COMMAND_FORMAT)

    print(f"total command count: {TOTAL_COUNT}")
    print(f"execute command: sh {EXEC_FILEPATH}")
