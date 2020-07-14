#ifndef MESSAGE_H
#define MESSAGE_H
#include "MainFrame.h"

namespace CommonTemplate{
		class Type {
		public:
			Type() : m_param(nullptr) {}
	template<typename type>
			void SetType(type t) {
				m_param = ((void *)t);
			}
			void* m_param;
		};

		class Params_Array {
		public:
			Params_Array() {};
	template<typename Type>
			void SaveType(Type t) {
				m_Type.SetType(t);
			}
			Type GetParam() {
				return m_Type;
			}
		private:
			Type m_Type;
		};

	template<int size>
		class RegisterType {
		public:
			RegisterType() {}
			void ExpandValues(void) {
				m_Types[size] = Params_Array();
			}
	template<typename T1, typename... Tn>
			void ExpandValues(T1 t, Tn... types) {
				m_Types[size - sizeof...(Tn) - 1].SaveType(t);
				ExpandValues(types...);
			}
			Params_Array* GetTypesArray() {
				return m_Types;
			}
		private:
			Params_Array m_Types[size + 1];
		};
		
		class InitType  {
		public:
			InitType() {}
	template<typename... Tn>
			InitType(Tn... types) {
				static RegisterType<sizeof...(Tn)> m_RegType; 
				m_RegType.ExpandValues(types...);
				m_Params = m_RegType.GetTypesArray();
				g_pSignal->SetUserIdentify(m_Params, SystemUser::MESSAGE);
			}	

	template<typename index>
			static void * Get(index index_) {
				Params_Array* pArray = (Params_Array*)g_pSignal->ReturnUser(SystemUser::MESSAGE);
				int i = 0;
				for (i; (pArray + i)->GetParam().m_param != nullptr; i++);
				if (index_ < i && index_ > -1) 
					return pArray[index_].GetParam().m_param;
				return nullptr;
			} 
			Params_Array* m_Params;
		};

	template<typename... Tn> 
			InitType* Send_Message(Tn... types) {
				static InitType Init(types...);
				return &Init;
			}
}


static bool Send_MessageThread(QString target, QString instance, CommonTemplate::InitType* init) {
	MainFrame* frame = (MainFrame*)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	return frame->SendMsgThread(target, instance, init);
}

#endif // MESSAGE_H