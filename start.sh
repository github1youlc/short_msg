#!/bin/bash
 ./bin/debug/ACCESS_LAYER > log/access_layer.log 2>&1 &
 ./bin/debug/SERVICE_ADD_FRIEND > log/service_add_friend.log 2>&1 &
 ./bin/debug/SERVICE_CONFIRM_ADD_FRIEND > log/service_confirm_add.log 2>&1 &
 ./bin/debug/SERVICE_GET_FRIENDS > log/service_get_friends.log 2>&1 &
 ./bin/debug/SERVICE_LOGIN > log/service_login.log 2>&1 &
 ./bin/debug/SERVICE_LOGOFF > log/service_logoff.log 2>&1 &
 ./bin/debug/SERVICE_REIGSTER > log/access_reigster.log 2>&1 &

