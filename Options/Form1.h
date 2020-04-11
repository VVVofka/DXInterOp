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
		cell0010 = gcnew Cell4(box0010);
		cell0011 = gcnew Cell4(box0011);
		cell0100 = gcnew Cell4(box0100);
		cell0101 = gcnew Cell4(box0101);
		cell0110 = gcnew Cell4(box0110);
		cell0111 = gcnew Cell4(box0111);

		cell1000 = gcnew Cell4(box1000);
		cell1001 = gcnew Cell4(box1001);
		cell1010 = gcnew Cell4(box1010);
		cell1011 = gcnew Cell4(box1011);
		cell1100 = gcnew Cell4(box1100);
		cell1101 = gcnew Cell4(box1101);
		cell1110 = gcnew Cell4(box1110);
		cell1111 = gcnew Cell4(box1111);
	}
private:
	Cell4
		^ cell0000, ^ cell0001, ^ cell0010, ^ cell0011, 
		^ cell0100, ^ cell0101, ^ cell0110, ^ cell0111, 
		^ cell1000, ^ cell1001, ^ cell1010, ^ cell1011,
		^ cell1100, ^ cell1101, ^ cell1110, ^ cell1111;
private: System::Windows::Forms::PictureBox^ box0010;
private: System::Windows::Forms::PictureBox^ box0100;
private: System::Windows::Forms::PictureBox^ box1000;
private: System::Windows::Forms::PictureBox^ box1010;
private: System::Windows::Forms::PictureBox^ box1100;
private: System::Windows::Forms::PictureBox^ box0101;
private: System::Windows::Forms::PictureBox^ box1001;
private: System::Windows::Forms::PictureBox^ box1011;
private: System::Windows::Forms::PictureBox^ box1101;
private: System::Windows::Forms::Button^ btn0001;
private: System::Windows::Forms::Button^ btn0011;
private: System::Windows::Forms::Button^ btn0111;
private: System::Windows::Forms::Button^ btn0110;



private: System::Windows::Forms::PictureBox^ box1110;
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
		this->box0010 = (gcnew System::Windows::Forms::PictureBox());
		this->box0100 = (gcnew System::Windows::Forms::PictureBox());
		this->box1000 = (gcnew System::Windows::Forms::PictureBox());
		this->box1010 = (gcnew System::Windows::Forms::PictureBox());
		this->box1100 = (gcnew System::Windows::Forms::PictureBox());
		this->box0101 = (gcnew System::Windows::Forms::PictureBox());
		this->box1001 = (gcnew System::Windows::Forms::PictureBox());
		this->box1011 = (gcnew System::Windows::Forms::PictureBox());
		this->box1101 = (gcnew System::Windows::Forms::PictureBox());
		this->box1110 = (gcnew System::Windows::Forms::PictureBox());
		this->btn0001 = (gcnew System::Windows::Forms::Button());
		this->btn0011 = (gcnew System::Windows::Forms::Button());
		this->btn0111 = (gcnew System::Windows::Forms::Button());
		this->btn0110 = (gcnew System::Windows::Forms::Button());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0110))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0000))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0001))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0011))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0111))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1111))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0010))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0100))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1000))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1010))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1100))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0101))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1001))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1011))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1101))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1110))->BeginInit();
		this->SuspendLayout();
		// 
		// box0110
		// 
		this->box0110->Location = System::Drawing::Point(263, 392);
		this->box0110->Name = L"box0110";
		this->box0110->Size = System::Drawing::Size(96, 96);
		this->box0110->TabIndex = 5;
		this->box0110->TabStop = false;
		this->box0110->Tag = L"0110";
		// 
		// box0000
		// 
		this->box0000->Location = System::Drawing::Point(25, 392);
		this->box0000->Name = L"box0000";
		this->box0000->Size = System::Drawing::Size(96, 96);
		this->box0000->TabIndex = 0;
		this->box0000->TabStop = false;
		this->box0000->Tag = L"0000";
		// 
		// box0001
		// 
		this->box0001->Location = System::Drawing::Point(25, 36);
		this->box0001->Name = L"box0001";
		this->box0001->Size = System::Drawing::Size(96, 96);
		this->box0001->TabIndex = 1;
		this->box0001->TabStop = false;
		this->box0001->Tag = L"0001";
		// 
		// box0011
		// 
		this->box0011->Location = System::Drawing::Point(25, 148);
		this->box0011->Name = L"box0011";
		this->box0011->Size = System::Drawing::Size(96, 96);
		this->box0011->TabIndex = 2;
		this->box0011->TabStop = false;
		this->box0011->Tag = L"0011";
		// 
		// box0111
		// 
		this->box0111->Location = System::Drawing::Point(25, 260);
		this->box0111->Name = L"box0111";
		this->box0111->Size = System::Drawing::Size(96, 96);
		this->box0111->TabIndex = 4;
		this->box0111->TabStop = false;
		this->box0111->Tag = L"0111";
		// 
		// box1111
		// 
		this->box1111->Location = System::Drawing::Point(144, 392);
		this->box1111->Name = L"box1111";
		this->box1111->Size = System::Drawing::Size(96, 96);
		this->box1111->TabIndex = 3;
		this->box1111->TabStop = false;
		this->box1111->Tag = L"1111";
		// 
		// box0010
		// 
		this->box0010->Location = System::Drawing::Point(174, 36);
		this->box0010->Name = L"box0010";
		this->box0010->Size = System::Drawing::Size(96, 96);
		this->box0010->TabIndex = 1;
		this->box0010->TabStop = false;
		this->box0010->Tag = L"0010";
		// 
		// box0100
		// 
		this->box0100->Location = System::Drawing::Point(293, 36);
		this->box0100->Name = L"box0100";
		this->box0100->Size = System::Drawing::Size(96, 96);
		this->box0100->TabIndex = 1;
		this->box0100->TabStop = false;
		this->box0100->Tag = L"0100";
		// 
		// box1000
		// 
		this->box1000->Location = System::Drawing::Point(412, 36);
		this->box1000->Name = L"box1000";
		this->box1000->Size = System::Drawing::Size(96, 96);
		this->box1000->TabIndex = 1;
		this->box1000->TabStop = false;
		this->box1000->Tag = L"1000";
		// 
		// box1010
		// 
		this->box1010->Location = System::Drawing::Point(174, 148);
		this->box1010->Name = L"box1010";
		this->box1010->Size = System::Drawing::Size(96, 96);
		this->box1010->TabIndex = 2;
		this->box1010->TabStop = false;
		this->box1010->Tag = L"1010";
		// 
		// box1100
		// 
		this->box1100->Location = System::Drawing::Point(293, 148);
		this->box1100->Name = L"box1100";
		this->box1100->Size = System::Drawing::Size(96, 96);
		this->box1100->TabIndex = 2;
		this->box1100->TabStop = false;
		this->box1100->Tag = L"1100";
		// 
		// box0101
		// 
		this->box0101->Location = System::Drawing::Point(412, 148);
		this->box0101->Name = L"box0101";
		this->box0101->Size = System::Drawing::Size(96, 96);
		this->box0101->TabIndex = 2;
		this->box0101->TabStop = false;
		this->box0101->Tag = L"0101";
		// 
		// box1001
		// 
		this->box1001->Location = System::Drawing::Point(412, 392);
		this->box1001->Name = L"box1001";
		this->box1001->Size = System::Drawing::Size(96, 96);
		this->box1001->TabIndex = 5;
		this->box1001->TabStop = false;
		this->box1001->Tag = L"1001";
		// 
		// box1011
		// 
		this->box1011->Location = System::Drawing::Point(174, 260);
		this->box1011->Name = L"box1011";
		this->box1011->Size = System::Drawing::Size(96, 96);
		this->box1011->TabIndex = 4;
		this->box1011->TabStop = false;
		this->box1011->Tag = L"1011";
		// 
		// box1101
		// 
		this->box1101->Location = System::Drawing::Point(412, 260);
		this->box1101->Name = L"box1101";
		this->box1101->Size = System::Drawing::Size(96, 96);
		this->box1101->TabIndex = 4;
		this->box1101->TabStop = false;
		this->box1101->Tag = L"1101";
		// 
		// box1110
		// 
		this->box1110->Location = System::Drawing::Point(293, 260);
		this->box1110->Name = L"box1110";
		this->box1110->Size = System::Drawing::Size(96, 96);
		this->box1110->TabIndex = 4;
		this->box1110->TabStop = false;
		this->box1110->Tag = L"1110";
		// 
		// btn0001
		// 
		this->btn0001->Location = System::Drawing::Point(127, 73);
		this->btn0001->Name = L"btn0001";
		this->btn0001->Size = System::Drawing::Size(41, 23);
		this->btn0001->TabIndex = 6;
		this->btn0001->Text = L">>";
		this->btn0001->UseVisualStyleBackColor = true;
		this->btn0001->Click += gcnew System::EventHandler(this, &Form1::btn0001_Click);
		// 
		// btn0011
		// 
		this->btn0011->Location = System::Drawing::Point(127, 185);
		this->btn0011->Name = L"btn0011";
		this->btn0011->Size = System::Drawing::Size(41, 23);
		this->btn0011->TabIndex = 6;
		this->btn0011->Text = L">>";
		this->btn0011->UseVisualStyleBackColor = true;
		// 
		// btn0111
		// 
		this->btn0111->Location = System::Drawing::Point(127, 297);
		this->btn0111->Name = L"btn0111";
		this->btn0111->Size = System::Drawing::Size(41, 23);
		this->btn0111->TabIndex = 6;
		this->btn0111->Text = L">>";
		this->btn0111->UseVisualStyleBackColor = true;
		// 
		// btn0110
		// 
		this->btn0110->Location = System::Drawing::Point(365, 429);
		this->btn0110->Name = L"btn0110";
		this->btn0110->Size = System::Drawing::Size(41, 23);
		this->btn0110->TabIndex = 6;
		this->btn0110->Text = L">>";
		this->btn0110->UseVisualStyleBackColor = true;
		// 
		// Form1
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(623, 662);
		this->Controls->Add(this->btn0110);
		this->Controls->Add(this->btn0111);
		this->Controls->Add(this->btn0011);
		this->Controls->Add(this->btn0001);
		this->Controls->Add(this->box0000);
		this->Controls->Add(this->box0010);
		this->Controls->Add(this->box0100);
		this->Controls->Add(this->box1000);
		this->Controls->Add(this->box0001);
		this->Controls->Add(this->box1010);
		this->Controls->Add(this->box1100);
		this->Controls->Add(this->box0101);
		this->Controls->Add(this->box0011);
		this->Controls->Add(this->box1111);
		this->Controls->Add(this->box1011);
		this->Controls->Add(this->box1110);
		this->Controls->Add(this->box1101);
		this->Controls->Add(this->box0111);
		this->Controls->Add(this->box1001);
		this->Controls->Add(this->box0110);
		this->Name = L"Form1";
		this->Text = L"Options";
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0110))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0000))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0001))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0011))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0111))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1111))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0010))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0100))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1000))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1010))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1100))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box0101))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1001))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1011))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1101))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->box1110))->EndInit();
		this->ResumeLayout(false);

	}
#pragma endregion

private: System::Void btn0001_Click(System::Object^ sender, System::EventArgs^ e){
	cell0010->Rotate(cell0001);
	box0010->Refresh();
} // /////////////////////////////////////////////////////////////////////////
};
}
