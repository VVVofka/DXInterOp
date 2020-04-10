#pragma once
#include "Cell4.h"

namespace CppCLRWinformsProjekt{

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/// <summary>
/// Zusammenfassung fьr Form1
/// </summary>
public ref class Form1 : public System::Windows::Forms::Form{
public:
	Form1(void){
		InitializeComponent();
		//TODO: Konstruktorcode hier hinzufьgen.
		cell0000 = gcnew Cell4(box0000);
		cell0001 = gcnew Cell4(box0001);
		cell0011 = gcnew Cell4(box0011);
		cell0110 = gcnew Cell4(box0110);
		cell0111 = gcnew Cell4(box0111);
		cell1111 = gcnew Cell4(box1111);
	}
private:
	   Cell4 ^cell0000, ^cell0001, ^cell0011, ^cell0110, ^ cell0111, ^ cell1111;
protected:
	/// <summary>
	/// Verwendete Ressourcen bereinigen.
	/// </summary>
	~Form1(){
		if(components){
			delete components;
		}
	}

private: System::Windows::Forms::PictureBox^ box0000;
private: System::Windows::Forms::PictureBox^ box0001;
private: System::Windows::Forms::PictureBox^ box0011;
private: System::Windows::Forms::PictureBox^ box0110;
private: System::Windows::Forms::PictureBox^ box0111;
private: System::Windows::Forms::PictureBox^ box1111;

private: System::ComponentModel::IContainer^ components;


protected:

private:
	/// <summary>
	/// Erforderliche Designervariable.
	/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode fьr die Designerunterstьtzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geдndert werden.
		/// </summary>
	void InitializeComponent(void){
		this->box0110 = (gcnew System::Windows::Forms::PictureBox());
		this->box0000 = (gcnew System::Windows::Forms::PictureBox());
		this->box0001 = (gcnew System::Windows::Forms::PictureBox());
		this->box0011 = (gcnew System::Windows::Forms::PictureBox());
		this->box0111 = (gcnew System::Windows::Forms::PictureBox());
		this->box1111 = (gcnew System::Windows::Forms::PictureBox());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0110))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0000))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0001))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0011))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0111))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1111))->BeginInit();
		this->SuspendLayout();
		// 
		// box0110
		// 
		this->box0110->Location = System::Drawing::Point(23, 326);
		this->box0110->Name = L"box0110";
		this->box0110->Size = System::Drawing::Size(96, 96);
		this->box0110->TabIndex = 2;
		this->box0110->TabStop = false;
		this->box0110->Tag = L"0110";
		// 
		// box0000
		// 
		this->box0000->Location = System::Drawing::Point(23, 20);
		this->box0000->Name = L"box0000";
		this->box0000->Size = System::Drawing::Size(96, 96);
		this->box0000->TabIndex = 2;
		this->box0000->TabStop = false;
		this->box0000->Tag = L"0000";
		// 
		// box0001
		// 
		this->box0001->Location = System::Drawing::Point(23, 122);
		this->box0001->Name = L"box0001";
		this->box0001->Size = System::Drawing::Size(96, 96);
		this->box0001->TabIndex = 2;
		this->box0001->TabStop = false;
		this->box0001->Tag = L"0001";
		// 
		// box0011
		// 
		this->box0011->Location = System::Drawing::Point(23, 224);
		this->box0011->Name = L"box0011";
		this->box0011->Size = System::Drawing::Size(96, 96);
		this->box0011->TabIndex = 2;
		this->box0011->TabStop = false;
		this->box0011->Tag = L"0011";
		// 
		// box0111
		// 
		this->box0111->Location = System::Drawing::Point(23, 428);
		this->box0111->Name = L"box0111";
		this->box0111->Size = System::Drawing::Size(96, 96);
		this->box0111->TabIndex = 2;
		this->box0111->TabStop = false;
		this->box0111->Tag = L"0111";
		// 
		// box1111
		// 
		this->box1111->Location = System::Drawing::Point(23, 530);
		this->box1111->Name = L"box1111";
		this->box1111->Size = System::Drawing::Size(96, 96);
		this->box1111->TabIndex = 2;
		this->box1111->TabStop = false;
		this->box1111->Tag = L"1111";
		// 
		// Form1
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(623, 662);
		this->Controls->Add(this->box0000);
		this->Controls->Add(this->box0001);
		this->Controls->Add(this->box0011);
		this->Controls->Add(this->box1111);
		this->Controls->Add(this->box0111);
		this->Controls->Add(this->box0110);
		this->Name = L"Form1";
		this->Text = L"Form1";
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0110))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0000))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0001))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0011))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0111))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1111))->EndInit();
		this->ResumeLayout(false);

	}
#pragma endregion
private: System::Void label1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e){
	auto x = sender;
	auto z = e;
}
private: System::Void label2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e){

}

};
}
