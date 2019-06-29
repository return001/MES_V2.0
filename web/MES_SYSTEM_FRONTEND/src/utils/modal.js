import {Notification} from 'element-ui'


/*
import eventBus from './eventBus'
function alertMsg (status, string, timeout, title) {
  eventBus.$emit('notify-me', {
    status: status,
    timeout: timeout,
    data: {
      title: title,
      text: string
    }
  })
}
*/


function alertMsg(type, message, timeout, title) {
  Notification({
    title: title,
    type: type,
    duration: timeout,
    message: message,
    position: "bottom-right"
  })
}


export const alertDanger = function (string) {
  alertMsg('error', string, 2000, '警告')
};


export const alertSuccess = function (string) {
  alertMsg('success', string, 2000, '成功')
};

export const alertInfo = function (string) {
  alertMsg('info', string, 2000, '提示')
};
export const alertWarning = function (string) {
  alertMsg('warning', string, 2000, '注意')
};
