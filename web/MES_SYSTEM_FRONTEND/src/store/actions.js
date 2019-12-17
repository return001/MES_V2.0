export const setLoginToken = ({commit}, token) => {
  commit('setLoginToken', token)
};

export const setTableRouter = ({commit}, tableRouterData) => {
  commit('setTableRouter', tableRouterData)
};

export const setRouter = ({commit}, routerIn) => {
  commit('setRouter', routerIn)
};
export const setEditing = ({commit}, isEditing) => {
  commit('setEditing', isEditing)
};
export const setEditData = ({commit}, editData) => {
  commit('setEditData', editData)
};
export const setCopyData = ({commit}, copyData) => {
  commit('setCopyData', copyData)
};
export const setTestType = ({commit}, testType) => {
  commit('setTestType', testType)
};
export const setUserType = ({commit}, userType) => {
  commit('setUserType', userType)
};
export const setUserTypeList = ({commit}, userTypeList) => {
  commit('setUserTypeList', userTypeList)
};

export const setDelPermission = ({commit}, list) => {
  commit('setDelPermission', list)
};

export const setTableDeleteGroup = ({commit}, object) => {
  commit('setTableDeleteGroup', object)
};
