export const deepCopy = obj => {
  let copyObj = Array.isArray(obj)?[]:{};
  for(let key in obj){
    copyObj[key] = typeof(obj[key])==="object"?deepCopy(obj[key]):obj[key];
  }
  return copyObj;
};