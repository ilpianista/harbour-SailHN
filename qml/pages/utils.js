.pragma library

function getRelativeTime(dateString) {
    var date = new Date(dateString)
    var now = new Date()
    var diffMs = now - date
    var diffSec = Math.floor(diffMs / 1000)
    var diffMin = Math.floor(diffSec / 60)
    var diffHour = Math.floor(diffMin / 60)
    var diffDay = Math.floor(diffHour / 24)

    if (diffSec < 60) {
        return qsTr("just now")
    } else if (diffMin < 60) {
        return diffMin === 1 ? qsTr("1 minute ago") : qsTr("%1 minutes ago").arg(diffMin)
    } else if (diffHour < 24) {
        return diffHour === 1 ? qsTr("1 hour ago") : qsTr("%1 hours ago").arg(diffHour)
    } else if (diffDay < 7) {
        return diffDay === 1 ? qsTr("1 day ago") : qsTr("%1 days ago").arg(diffDay)
    } else {
        return Qt.formatDateTime(date)
    }
}
