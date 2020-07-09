import {deepCopy} from "../utils/utils";

export const setLoginToken = (state, token) => {
  state.token = token;
};
export const setSessionFactory = (state, factory) => {
  state.factory = factory;
};

export const setRouter = (state, routerIn) => {
  state.routerIn = routerIn
};
export const setEditing = (state, isEditing) => {
  state.isEditing = isEditing
};
export const setEditData = (state, editData) => {
  state.editData = editData
};
export const setCopyData = (state, copyData) => {
  state.copyData = copyData
};
export const setTestType = (state, testType) => {
  state.testType = testType
};
export const setUserType = (state, userType) => {
  state.userType = userType
};
export const setUserTypeList = (state, userTypeList) => {
  Object.assign(state.userTypeList, userTypeList);
};
export const setDelPermission = (state, list) => {
  Object.assign(state.delPermission, list);
};

export const setTableDeleteGroup = (state, obj) => {
  state.tableDeleteGroup[obj.key] = obj.val
};

export const setStashData = (state, obj) => {
  state.stashData = deepCopy(obj);
};

export const setCharactersFuncMap = (state, obj) => {
  state.charactersFuncMap = deepCopy(obj);
};
export let pageActionLimits = (state, limitList)=>{
  state.limits = {
        select:limitList[0],
        add:limitList[1],
        update:limitList[2],
        delete:limitList[3],
        upload:limitList[4],
        download:limitList[5],
        checkout:limitList[6],
        other:limitList[7],
        explain:limitList[8],
      }
}
