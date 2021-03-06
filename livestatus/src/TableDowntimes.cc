// Copyright (C) 2019 tribe29 GmbH - License: GNU General Public License v2
// This file is part of Checkmk (https://checkmk.com). It is subject to the
// terms and conditions defined in the file COPYING, which is part of this
// source code package.

#include "TableDowntimes.h"

#include <memory>
#include <utility>

#include "Column.h"
#include "DowntimeOrComment.h"
#include "DowntimesOrComments.h"
#include "MonitoringCore.h"
#include "OffsetBoolColumn.h"
#include "OffsetIntColumn.h"
#include "OffsetSStringColumn.h"
#include "OffsetTimeColumn.h"
#include "Query.h"
#include "Row.h"
#include "Store.h"
#include "TableHosts.h"
#include "TableServices.h"
#include "auth.h"
#include "nagios.h"

// TODO(sp): the dynamic data in this table must be locked with a mutex

TableDowntimes::TableDowntimes(MonitoringCore *mc) : Table(mc) {
    addColumn(std::make_unique<OffsetSStringColumn>(
        "author", "The contact that scheduled the downtime",
        Column::Offsets{-1, -1, -1,
                        DANGEROUS_OFFSETOF(Downtime, _author_name)}));
    addColumn(std::make_unique<OffsetSStringColumn>(
        "comment", "A comment text",
        Column::Offsets{-1, -1, -1, DANGEROUS_OFFSETOF(Downtime, _comment)}));
    addColumn(std::make_unique<OffsetIntColumn>(
        "id", "The id of the downtime",
        Column::Offsets{-1, -1, -1, DANGEROUS_OFFSETOF(Downtime, _id)}));
    addColumn(std::make_unique<OffsetTimeColumn>(
        "entry_time", "The time the entry was made as UNIX timestamp",
        Column::Offsets{-1, -1, -1,
                        DANGEROUS_OFFSETOF(Downtime, _entry_time)}));
    addColumn(std::make_unique<OffsetIntColumn>(
        "type",
        "The type of the downtime: 0 if it is active, 1 if it is pending",
        Column::Offsets{-1, -1, -1, DANGEROUS_OFFSETOF(Downtime, _type)}));
    addColumn(std::make_unique<OffsetBoolColumn>(
        "is_service",
        "0, if this entry is for a host, 1 if it is for a service",
        Column::Offsets{-1, -1, -1,
                        DANGEROUS_OFFSETOF(Downtime, _is_service)}));

    addColumn(std::make_unique<OffsetTimeColumn>(
        "start_time", "The start time of the downtime as UNIX timestamp",
        Column::Offsets{-1, -1, -1,
                        DANGEROUS_OFFSETOF(Downtime, _start_time)}));
    addColumn(std::make_unique<OffsetTimeColumn>(
        "end_time", "The end time of the downtime as UNIX timestamp",
        Column::Offsets{-1, -1, -1, DANGEROUS_OFFSETOF(Downtime, _end_time)}));
    addColumn(std::make_unique<OffsetIntColumn>(
        "fixed", "A 1 if the downtime is fixed, a 0 if it is flexible",
        Column::Offsets{-1, -1, -1, DANGEROUS_OFFSETOF(Downtime, _fixed)}));
    addColumn(std::make_unique<OffsetIntColumn>(
        "duration", "The duration of the downtime in seconds",
        Column::Offsets{-1, -1, -1, DANGEROUS_OFFSETOF(Downtime, _duration)}));
    addColumn(std::make_unique<OffsetIntColumn>(
        "triggered_by",
        "The id of the downtime this downtime was triggered by or 0 if it was not triggered by another downtime",
        Column::Offsets{-1, -1, -1,
                        DANGEROUS_OFFSETOF(Downtime, _triggered_by)}));

    TableHosts::addColumns(this, "host_", DANGEROUS_OFFSETOF(Downtime, _host),
                           -1);
    TableServices::addColumns(this, "service_",
                              DANGEROUS_OFFSETOF(Downtime, _service),
                              false /* no hosts table */);
}

std::string TableDowntimes::name() const { return "downtimes"; }

std::string TableDowntimes::namePrefix() const { return "downtime_"; }

void TableDowntimes::answerQuery(Query *query) {
    for (const auto &entry : core()->impl<Store>()->_downtimes) {
        if (!query->processDataset(Row(entry.second.get()))) {
            break;
        }
    }
}

bool TableDowntimes::isAuthorized(Row row, const contact *ctc) const {
    const auto *dtc = rowData<DowntimeOrComment>(row);
    return is_authorized_for(core(), ctc, dtc->_host, dtc->_service);
}
