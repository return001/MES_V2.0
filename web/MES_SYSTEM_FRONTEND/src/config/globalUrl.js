let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}


export const loginUrl = url + '/mes_server/user/login';
export const logoutUrl = url + '/mes_server/user/logout';
export const userUpdateUrl = url + '/mes_server/user/update';
export const userAddUrl = url + '/mes_server/user/add';
export const userQueryUrl = url + '/mes_server/user/select';

export const redTeaUrl = url + '/mes_server/redTea/findCId';

export const tableDownloadUrl = url + '/mes_server/report/download';

export const relationUpdateUrl = url + '/mes_server/report/updateRelativeSheet';
