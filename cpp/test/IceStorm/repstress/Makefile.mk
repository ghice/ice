# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(test)_programs        = publisher subscriber control
$(test)_dependencies    = IceStorm Ice TestCommon

$(test)_publisher_sources       = Publisher.cpp Single.ice Controller.ice
$(test)_subscriber_sources      = Subscriber.cpp Single.ice Controller.ice
$(test)_control_sources         = Control.cpp Controller.ice

$(test)_cleanfiles = 0.db/* 1.db/* 2.db/*

tests += $(test)
