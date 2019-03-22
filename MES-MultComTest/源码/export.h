#pragma once

#ifndef EXPORT_H
#define EXPORT_H

#ifndef EXPORT_DLL

#define EXPORT_API extern "C" __declspec(dllimport)
//#define EXPORT_API  _declspec(dllexport)
#else 
#define  EXPORT_API _declspec(dllexport)

#endif // !EXPORT_DLL



#endif //EXPORT_H