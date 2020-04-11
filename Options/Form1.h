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
	void setDir(int a, int ncell, int nitem, int dir){ cells[a]->getCell(ncell, nitem)->setDir(dir); }
	int getDir(int a, int ncell, int nitem){ return cells[a]->getCell(ncell, nitem)->getDir(); }
	Form1(void){
		InitializeComponent();
		//TODO: Konstruktorcode hier hinzufьgen.
		cells[0b0000] = gcnew Cell4(box0000);
		cells[0b0001] = gcnew Cell4(box0001);
		cells[0b0010] = gcnew Cell4(box0010);
		cells[0b0011] = gcnew Cell4(box0011);
		cells[0b0100] = gcnew Cell4(box0100);
		cells[0b0101] = gcnew Cell4(box0101);
		cells[0b0110] = gcnew Cell4(box0110);
		cells[0b0111] = gcnew Cell4(box0111);

		cells[0b1000] = gcnew Cell4(box1000);
		cells[0b1001] = gcnew Cell4(box1001);
		cells[0b1010] = gcnew Cell4(box1010);
		cells[0b1011] = gcnew Cell4(box1011);
		cells[0b1100] = gcnew Cell4(box1100);
		cells[0b1101] = gcnew Cell4(box1101);
		cells[0b1110] = gcnew Cell4(box1110);
		cells[0b1111] = gcnew Cell4(box1111);
	}
private:
	array<Cell4^>^ cells = gcnew array<Cell4^>(16);
private: System::Windows::Forms::PictureBox^ box0010;
private: System::Windows::Forms::PictureBox^ box0100;
private: System::Windows::Forms::PictureBox^ box1000;
private: System::Windows::Forms::PictureBox^ box1010;
private: System::Windows::Forms::PictureBox^ box1100;
private: System::Windows::Forms::PictureBox^ box0101;
private: System::Windows::Forms::PictureBox^ box1001;
private: System::Windows::Forms::PictureBox^ box1011;
private: System::Windows::Forms::PictureBox^ box1101;
private: System::Windows::Forms::PictureBox^ box0000;
private: System::Windows::Forms::PictureBox^ box0001;
private: System::Windows::Forms::PictureBox^ box0011;
private: System::Windows::Forms::PictureBox^ box0110;
private: System::Windows::Forms::PictureBox^ box0111;
private: System::Windows::Forms::PictureBox^ box1111;
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
		this->box0110->Tag = L"0110";
		// 
		// box0000
		// 
		this->box0000->Location = System::Drawing::Point(25, 392);
		this->box0000->Name = L"box0000";
		this->box0000->Size = System::Drawing::Size(96, 96);
		this->box0000->Tag = L"0000";
		// 
		// box0001
		// 
		this->box0001->Location = System::Drawing::Point(25, 36);
		this->box0001->Name = L"box0001";
		this->box0001->Size = System::Drawing::Size(96, 96);
		this->box0001->Tag = L"0001";
		// 
		// box0011
		// 
		this->box0011->Location = System::Drawing::Point(25, 148);
		this->box0011->Name = L"box0011";
		this->box0011->Size = System::Drawing::Size(96, 96);
		this->box0011->Tag = L"0011";
		// 
		// box0111
		// 
		this->box0111->Location = System::Drawing::Point(25, 260);
		this->box0111->Name = L"box0111";
		this->box0111->Size = System::Drawing::Size(96, 96);
		this->box0111->Tag = L"0111";
		// 
		// box1111
		// 
		this->box1111->Location = System::Drawing::Point(144, 392);
		this->box1111->Name = L"box1111";
		this->box1111->Size = System::Drawing::Size(96, 96);
		this->box1111->Tag = L"1111";
		// 
		// box0010
		// 
		this->box0010->Location = System::Drawing::Point(174, 36);
		this->box0010->Name = L"box0010";
		this->box0010->Size = System::Drawing::Size(96, 96);
		this->box0010->Tag = L"0010";
		// 
		// box0100
		// 
		this->box0100->Location = System::Drawing::Point(412, 36);
		this->box0100->Name = L"box0100";
		this->box0100->Size = System::Drawing::Size(96, 96);
		this->box0100->Tag = L"0100";
		// 
		// box1000
		// 
		this->box1000->Location = System::Drawing::Point(293, 36);
		this->box1000->Name = L"box1000";
		this->box1000->Size = System::Drawing::Size(96, 96);
		this->box1000->Tag = L"1000";
		// 
		// box1010
		// 
		this->box1010->Location = System::Drawing::Point(174, 148);
		this->box1010->Name = L"box1010";
		this->box1010->Size = System::Drawing::Size(96, 96);
		this->box1010->Tag = L"1010";
		// 
		// box1100
		// 
		this->box1100->Location = System::Drawing::Point(293, 148);
		this->box1100->Name = L"box1100";
		this->box1100->Size = System::Drawing::Size(96, 96);
		this->box1100->Tag = L"1100";
		// 
		// box0101
		// 
		this->box0101->Location = System::Drawing::Point(412, 148);
		this->box0101->Name = L"box0101";
		this->box0101->Size = System::Drawing::Size(96, 96);
		this->box0101->Tag = L"0101";
		// 
		// box1001
		// 
		this->box1001->Location = System::Drawing::Point(412, 392);
		this->box1001->Name = L"box1001";
		this->box1001->Size = System::Drawing::Size(96, 96);
		this->box1001->Tag = L"1001";
		// 
		// box1011
		// 
		this->box1011->Location = System::Drawing::Point(174, 260);
		this->box1011->Name = L"box1011";
		this->box1011->Size = System::Drawing::Size(96, 96);
		this->box1011->Tag = L"1011";
		// 
		// box1101
		// 
		this->box1101->Location = System::Drawing::Point(412, 260);
		this->box1101->Name = L"box1101";
		this->box1101->Size = System::Drawing::Size(96, 96);
		this->box1101->Tag = L"1101";
		// 
		// box1110
		// 
		this->box1110->Location = System::Drawing::Point(293, 260);
		this->box1110->Name = L"box1110";
		this->box1110->Size = System::Drawing::Size(96, 96);
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
		this->btn0011->Click += gcnew System::EventHandler(this, &Form1::btn0011_Click);
		// 
		// btn0111
		// 
		this->btn0111->Location = System::Drawing::Point(127, 297);
		this->btn0111->Name = L"btn0111";
		this->btn0111->Size = System::Drawing::Size(41, 23);
		this->btn0111->TabIndex = 6;
		this->btn0111->Text = L">>";
		this->btn0111->UseVisualStyleBackColor = true;
		this->btn0111->Click += gcnew System::EventHandler(this, &Form1::btn0111_Click);
		// 
		// btn0110
		// 
		this->btn0110->Location = System::Drawing::Point(365, 429);
		this->btn0110->Name = L"btn0110";
		this->btn0110->Size = System::Drawing::Size(41, 23);
		this->btn0110->TabIndex = 6;
		this->btn0110->Text = L">>";
		this->btn0110->UseVisualStyleBackColor = true;
		this->btn0110->Click += gcnew System::EventHandler(this, &Form1::btn0110_Click);
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
	cells[0b0010]->Rotate(cells[0b0001]);
	box0010->Refresh();

	cells[0b1000]->Rotate(cells[0b0010]);
	box1000->Refresh();

	cells[0b0100]->Rotate(cells[0b1000]);
	box0100->Refresh();
} // /////////////////////////////////////////////////////////////////////////
private: System::Void btn0011_Click(System::Object^ sender, System::EventArgs^ e){
	cells[0b1010]->Rotate(cells[0b0011]);
	box1010->Refresh();

	cells[0b1100]->Rotate(cells[0b1010]);
	box1100->Refresh();

	cells[0b0101]->Rotate(cells[0b1100]);
	box0101->Refresh();
} // /////////////////////////////////////////////////////////////////////////
private: System::Void btn0111_Click(System::Object^ sender, System::EventArgs^ e){
	cells[0b1011]->Rotate(cells[0b0111]);
	box1011->Refresh();

	cells[0b1110]->Rotate(cells[0b1011]);
	box1110->Refresh();

	cells[0b1101]->Rotate(cells[0b1110]);
	box1101->Refresh();
} // /////////////////////////////////////////////////////////////////////////
private: System::Void btn0110_Click(System::Object^ sender, System::EventArgs^ e){
	cells[0b1001]->Rotate(cells[0b0110]);
	box1001->Refresh();
} // /////////////////////////////////////////////////////////////////////////


};
}
