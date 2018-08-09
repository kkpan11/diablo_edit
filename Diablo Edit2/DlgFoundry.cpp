// CDlgFoundry.cpp: 实现文件
//

#include "stdafx.h"
#include "Diablo Edit2.h"
#include "DlgFoundry.h"
#include "afxdialogex.h"

using namespace std;

// CDlgFoundry 对话框

IMPLEMENT_DYNAMIC(CDlgFoundry, CDialogEx)

CDlgFoundry::CDlgFoundry(CD2Item & item, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_Foundry, pParent)
	, m_stItem(item)
	, m_bUnidentified(FALSE), m_bEthereal(FALSE), m_sExtSockets(_T(""))
{}

CDlgFoundry::~CDlgFoundry() {}

void CDlgFoundry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LEVEL, m_sText[0]);
	DDX_Text(pDX, IDC_STATIC_OWNER, m_sText[1]);
	DDX_Text(pDX, IDC_STATIC_CLASS, m_sText[2]);
	DDX_Text(pDX, IDC_CHECK_UNIDENTIFIED, m_sText[3]);
	DDX_Text(pDX, IDC_CHECK_ETHEREAL, m_sText[4]);
	DDX_Text(pDX, IDC_STATIC_DURABILITY, m_sText[5]);
	DDX_Text(pDX, IDC_CHECK_INDESTRUCTIBLE, m_sText[6]);
	DDX_Text(pDX, IDC_STATIC_QUANTITY, m_sText[7]);
	DDX_Text(pDX, IDC_STATIC_QUALITY, m_sText[8]);
	DDX_Text(pDX, IDC_STATIC_Sockets, m_sText[9]);
	DDX_Text(pDX, IDC_STATIC_DEFENCE, m_sText[10]);
	DDX_Text(pDX, IDC_STATIC_MONSTER, m_sText[11]);
	DDX_Text(pDX, IDC_STATIC_FIRST_NAME, m_sText[12]);
	DDX_Text(pDX, IDC_STATIC_SECOND_NAME, m_sText[13]);
	DDX_Text(pDX, IDC_STATIC_SUBTYPE, m_sText[14]);
	DDX_Text(pDX, IDC_STATIC_PREFIX1, m_sText[15]);
	DDX_Text(pDX, IDC_STATIC_SUFFIX1, m_sText[16]);
	DDX_Text(pDX, IDC_STATIC_SET_NAME, m_sText[17]);
	DDX_Text(pDX, IDC_STATIC_UNIQUE_NAME, m_sText[18]);
	DDX_Control(pDX, IDC_COMBO_CLASS, m_cbClass);
	DDX_Control(pDX, IDC_COMBO_QUALITY, m_cbQuality);
	DDX_Control(pDX, IDC_COMBO_MONSTER, m_cbMonster);
	DDX_Control(pDX, IDC_COMBO_FIRST_NAME, m_cbRareCraftedName[0]);
	DDX_Control(pDX, IDC_COMBO_SECOND_NAME, m_cbRareCraftedName[1]);
	DDX_Control(pDX, IDC_COMBO_SUBTYPE, m_cbSubType);
	DDX_Control(pDX, IDC_COMBO_PREFX1, m_cbPrefix[0]);
	DDX_Control(pDX, IDC_COMBO_PREFIX2, m_cbPrefix[1]);
	DDX_Control(pDX, IDC_COMBO_PREFIX3, m_cbPrefix[2]);
	DDX_Control(pDX, IDC_COMBO_SUFFIX1, m_cbSuffix[0]);
	DDX_Control(pDX, IDC_COMBO_SUFFIX2, m_cbSuffix[1]);
	DDX_Control(pDX, IDC_COMBO_SUFFIX3, m_cbSuffix[2]);
	DDX_Control(pDX, IDC_COMBO_SET_NAME, m_cbSetName);
	DDX_Control(pDX, IDC_COMBO_UNIQUE_NAME, m_cbUniqueName);
	DDX_Control(pDX, IDC_EDIT_LEVEL, m_edLevel);
	DDX_Check(pDX, IDC_CHECK_UNIDENTIFIED, m_bUnidentified);
	DDX_Check(pDX, IDC_CHECK_ETHEREAL, m_bEthereal);
	DDX_Control(pDX, IDC_CHECK_INDESTRUCTIBLE, m_chIndestructible);
	DDX_Control(pDX, IDC_EDIT_DURABILITY_MAX, m_edMaxDurability);
	DDX_Control(pDX, IDC_EDIT_DURABILITY, m_edDurability);
	DDX_Control(pDX, IDC_EDIT_QUANTITY, m_edQuantity);
	DDX_Control(pDX, IDC_EDIT_OWNER, m_edOwner);
	DDX_Control(pDX, IDC_EDIT_BASE_SOCKETS, m_edSockets);
	DDX_Text(pDX, IDC_STATIC_SOCKETS_EXT, m_sExtSockets);
	DDX_Control(pDX, IDC_EDIT_DEFENCE, m_edDefence);
	DDX_Control(pDX, IDC_TAB_PROPERTY, m_tabPropList);
	DDX_Control(pDX, IDC_LIST1, m_lstProperty);
	DDX_Control(pDX, IDC_EDIT1, m_edParam[0]);
	DDX_Control(pDX, IDC_EDIT2, m_edParam[1]);
	DDX_Control(pDX, IDC_EDIT3, m_edParam[2]);
	DDX_Control(pDX, IDC_EDIT4, m_edParam[3]);
	DDX_Text(pDX, IDC_STATIC_TIP1, m_sTip[0]);
	DDX_Text(pDX, IDC_STATIC_TIP2, m_sTip[1]);
	DDX_Text(pDX, IDC_STATIC_TIP3, m_sTip[2]);
	DDX_Text(pDX, IDC_STATIC_TIP4, m_sTip[3]);
}


BEGIN_MESSAGE_MAP(CDlgFoundry, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_INDESTRUCTIBLE, &CDlgFoundry::OnBnClickedCheckIndestructible)
	ON_CBN_SELCHANGE(IDC_COMBO_QUALITY, &CDlgFoundry::OnCbnSelchangeComboQuality)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROPERTY, &CDlgFoundry::OnTcnSelchangeTabProperty)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgFoundry::OnLvnItemchangedList1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFoundry::OnEnChangeParam)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgFoundry::OnEnChangeParam)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgFoundry::OnEnChangeParam)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgFoundry::OnEnChangeParam)
END_MESSAGE_MAP()


// CDlgFoundry 消息处理程序

void CDlgFoundry::InitUI() {
	//UI text
	int idx = 0;
	for (int i = 0; i < int(size(m_sText)); ++i)
		m_sText[i] = ::theApp.FoundryUI(idx++);
	for (int i = 0; i < int(size(m_aProps)); ++i)
		m_tabPropList.InsertItem(i, ::theApp.FoundryUI(idx++));
		//UI combox
	for (UINT i = 0; i < ::theApp.CLASS_NAME_SIZE; ++i)
		m_cbClass.AddString(::theApp.ClassName(i));
	for (UINT i = 0; i < ::theApp.ItemQualityNameSize(); ++i)
		m_cbQuality.AddString(::theApp.ItemQualityName(i));
	for (UINT i = 0; i < ::theApp.MonsterNameSize(); ++i)
		m_cbMonster.AddString(::theApp.MonsterName(i));
	for (UINT i = 0; i < ::theApp.RareCraftedNameSize(); ++i) {
		const auto name = ::theApp.RareCraftedName(i);
		m_cbRareCraftedName[0].InsertString(-1, name);
		m_cbRareCraftedName[1].InsertString(-1, name);
	}
	WCHAR tmp[] = _T("0");
	for (; tmp[0] < _T('8'); ++tmp[0])
		m_cbSubType.InsertString(-1, tmp);
	for (UINT i = 0; i < ::theApp.MagicPrefixSize(); ++i) {
		const auto str = ::theApp.MagicPrefix(i);
		for (auto & cb : m_cbPrefix)
			cb.InsertString(-1, str);
	}
	for (UINT i = 0; i < ::theApp.MagicSuffixSize(); ++i) {
		const auto str = ::theApp.MagicSuffix(i);
		for (auto & cb : m_cbSuffix)
			cb.InsertString(-1, str);
	}
	for (UINT i = 0; i < ::theApp.SetItemNameSize(); ++i)
		m_cbSetName.InsertString(-1, ::theApp.SetItemName(i));
	for (UINT i = 0; i < ::theApp.UniqueNameSize(); ++i)
		m_cbUniqueName.InsertString(-1, ::theApp.UniqueName(i));
	//Property list
	m_lstProperty.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lstProperty.InsertColumn(0, ::theApp.FoundryUI(idx++), LVCFMT_LEFT, 60);
	m_lstProperty.InsertColumn(1, ::theApp.FoundryUI(idx++), LVCFMT_LEFT, 300);
	for (UINT i = 0; i < ::theApp.PropertyNameSize(); ++i) {
		m_lstProperty.InsertItem(i, CSFormat(_T("%3d"), i));				//属性ID
		m_lstProperty.SetItemText(i, 1, ::theApp.PropertyDescription(i));	//属性描述
	}
}

void CDlgFoundry::ReadItemEar(const CEar & ear) {
	m_edLevel.EnableWindow(TRUE);
	m_edLevel.SetWindowText(CSFormat(_T("%d"), ear.iEarLevel));
	m_edOwner.EnableWindow(TRUE);
	m_edOwner.SetWindowText(CString(ear.sEarName));
	m_cbClass.EnableWindow(TRUE);
	m_cbClass.SetCurSel(ear.iEarClass);
}

void CDlgFoundry::ReadItemGold(const CGoldQuantity & gold) {
	m_edQuantity.EnableWindow(TRUE);
	m_edQuantity.SetWindowText(CSFormat(_T("%d"), int(gold.wQuantity)));
}

void CDlgFoundry::ReadItemLongName(const CLongName & longName) {
	m_cbRareCraftedName[0].SetCurSel(longName.iName1);
	m_cbRareCraftedName[1].SetCurSel(longName.iName2);
	m_cbPrefix[0].SetCurSel(longName.wPref1.exist() ? longName.wPref1 : 0);
	m_cbPrefix[1].SetCurSel(longName.wPref2.exist() ? longName.wPref2 : 0);
	m_cbPrefix[2].SetCurSel(longName.wPref3.exist() ? longName.wPref3 : 0);
	m_cbSuffix[0].SetCurSel(longName.wSuff1.exist() ? longName.wSuff1 : 0);
	m_cbSuffix[1].SetCurSel(longName.wSuff2.exist() ? longName.wSuff2 : 0);
	m_cbSuffix[2].SetCurSel(longName.wSuff3.exist() ? longName.wSuff3 : 0);
}

void CDlgFoundry::ReadExtItemInfo(const CExtItemInfo & extInfo) {
	m_edLevel.EnableWindow(TRUE);
	m_edLevel.SetWindowText(CSFormat(_T("%d"), int(extInfo.iDropLevel)));
	ASSERT(0 < extInfo.iQuality);
	m_cbQuality.EnableWindow(TRUE);
	m_cbQuality.SetCurSel(extInfo.iQuality - 1);
	OnCbnSelchangeComboQuality();
	switch (extInfo.iQuality) {
		case 1:		//low
			ASSERT(extInfo.loQual.exist());
			m_cbSubType.SetCurSel(extInfo.loQual);
			break;
		case 3:		//high
			ASSERT(extInfo.hiQual.exist());
			m_cbSubType.SetCurSel(extInfo.hiQual);
			break;
		case 4:		//magical
			ASSERT(extInfo.wPrefix.exist());
			ASSERT(extInfo.wSuffix.exist());
			m_cbPrefix[0].SetCurSel(extInfo.wPrefix);
			m_cbSuffix[0].SetCurSel(extInfo.wSuffix);
			break;
		case 5:		//set
			ASSERT(extInfo.wSetID.exist());
			m_cbSetName.SetCurSel(extInfo.wSetID);
			break;
		case 6:		//rare
			ASSERT(extInfo.pRareName.exist());
			ReadItemLongName(*extInfo.pRareName);
			break;
		case 7:		//unique
			ASSERT(extInfo.wUniID.exist());
			m_cbUniqueName.SetCurSel(extInfo.wUniID);
			break;
		case 8:		//crafted
			ASSERT(extInfo.pCraftName.exist());
			ReadItemLongName(*extInfo.pCraftName);
			break;
		default:;	//normal
	}
	m_edOwner.EnableWindow(TRUE);
	if (extInfo.sPersonName.exist())
		m_edOwner.SetWindowText(CString(extInfo.sPersonName));
	if (extInfo.wMonsterID.exist()) {
		m_cbMonster.EnableWindow(TRUE);
		m_cbMonster.SetCurSel(extInfo.wMonsterID);
	}
}

void CDlgFoundry::ReadTypeSpecificInfo(CTypeSpecificInfo & spec) {
	if (spec.iDefence.exist()) {
		m_edDefence.EnableWindow(TRUE);
		m_edDefence.SetWindowText(CSFormat(_T("%d"), spec.GetDefence()));
	}
	if (spec.iMaxDurability.exist()) {
		m_edMaxDurability.SetWindowText(CSFormat(_T("%d"), int(spec.iMaxDurability)));
		m_edDurability.SetWindowText(CSFormat(_T("%d"), spec.iCurDur.exist() ? int(spec.iCurDur) : 0));
		m_chIndestructible.EnableWindow(TRUE);
		m_chIndestructible.SetCheck(spec.IsIndestructible());
		if (!m_chIndestructible.GetCheck()) {
			m_edMaxDurability.EnableWindow(TRUE);
			m_edDurability.EnableWindow(TRUE);
		}
	}
	m_edSockets.EnableWindow(TRUE);
	const auto s = spec.Sockets();
	m_edSockets.SetWindowText(CSFormat(_T("%d"), s.first));
	m_sExtSockets = CSFormat(_T("%d"), s.second);
	if (spec.iQuantity.exist()) {
		m_edQuantity.EnableWindow(TRUE);
		m_edQuantity.SetWindowText(CSFormat(_T("%d"), int(spec.iQuantity)));
	}
	m_aProps[0] = { spec.stPropertyList, TRUE };
	for (int i = 1; i <= 5; ++i)
		if (spec.apSetProperty[i - 1].exist())
			m_aProps[i].first = *spec.apSetProperty[i - 1];
	if (spec.stRuneWordPropertyList.exist())
		m_aProps[6] = { *spec.stRuneWordPropertyList, TRUE };
}

void CDlgFoundry::ReadItemInfo(CItemInfo & info) {
	if (info.pGold.exist())
		return ReadItemGold(*info.pGold);
	if (info.pExtItemInfo.exist())
		ReadExtItemInfo(*info.pExtItemInfo);
	if (info.pTpSpInfo.exist())
		ReadTypeSpecificInfo(*info.pTpSpInfo);
}

void CDlgFoundry::ReadItem() {
	SetWindowText(m_stItem.ItemName());
	m_bUnidentified = !m_stItem.bIdentified;
	m_bEthereal = m_stItem.bEthereal;
	if (m_stItem.pEar.exist())			//耳朵
		return ReadItemEar(*m_stItem.pEar);
	//普通物品
	ASSERT(m_stItem.pItemInfo.exist());
	ReadItemInfo(*m_stItem.pItemInfo);
}

BOOL CDlgFoundry::OnInitDialog() {
	CDialogEx::OnInitDialog();
	InitUI();
	ReadItem();
	UpdatePropList();
	UpdateData(FALSE);
	return TRUE;
}

void CDlgFoundry::OnBnClickedCheckIndestructible() {
	const BOOL ch = m_chIndestructible.GetCheck();
	m_edDurability.EnableWindow(!ch);
	m_edMaxDurability.EnableWindow(!ch);
}

void CDlgFoundry::OnCbnSelchangeComboQuality() {
	//Disable all combox
	for (auto & cb : m_cbRareCraftedName)
		cb.EnableWindow(FALSE);
	for (auto & cb : m_cbPrefix)
		cb.EnableWindow(FALSE);
	for (auto & cb : m_cbSuffix)
		cb.EnableWindow(FALSE);
	m_cbSubType.EnableWindow(FALSE);
	m_cbSetName.EnableWindow(FALSE);
	m_cbUniqueName.EnableWindow(FALSE);
	//Set props
	for (int i = 0; i < 5; ++i)
		m_aProps[i + 1].second = (m_cbQuality.GetCurSel() == 4);
	switch (m_cbQuality.GetCurSel()) {
		case 0:case 2:	//low & high
			m_cbSubType.EnableWindow(TRUE);
			break;
		case 3:			//magical
			m_cbPrefix[0].EnableWindow(TRUE);
			m_cbSuffix[0].EnableWindow(TRUE);
			break;
		case 4:			//set
			m_cbSetName.EnableWindow(TRUE);
			break;
		case 5:case 7:	//rare
			for (auto & cb : m_cbRareCraftedName)
				cb.EnableWindow(TRUE);
			for (auto & cb : m_cbPrefix)
				cb.EnableWindow(TRUE);
			for (auto & cb : m_cbSuffix)
				cb.EnableWindow(TRUE);
			break;
		case 6:			//unique
			m_cbUniqueName.EnableWindow(TRUE);
			break;
		default:;		//normal
	}
}

void CDlgFoundry::OnTcnSelchangeTabProperty(NMHDR *pNMHDR, LRESULT *pResult) {
	UpdatePropList();
	UpdateData(FALSE);
	*pResult = 0;
}

static int CALLBACK PropItemCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	auto & list = *reinterpret_cast<const CListCtrl *>(lParamSort);
	const auto i = static_cast<int>(lParam1), j = static_cast<int>(lParam2);
	const auto ch1 = list.GetCheck(i), ch2 = list.GetCheck(j);
	if (ch1 != ch2)
		return (ch1 ? -1 : 1);
	const auto s1 = list.GetItemText(i, 0), s2 = list.GetItemText(j, 0);
	return (s1 < s2 ? -1 : 1);
}

void CDlgFoundry::UpdatePropList() {
	//reset params
	m_lstProperty.SetItemState(m_iSelectedPropItem, 0, LVIS_SELECTED);
	m_iSelectedPropItem = -1;
	SetupPropParameters(-1, 0);
	//uncheck all items
	for (int i = 0; i < m_lstProperty.GetItemCount(); ++i)
		if (m_lstProperty.GetCheck(i)) {
			m_lstProperty.SetCheck(i, FALSE);
			const auto code = _tstoi(m_lstProperty.GetItemText(i, 0).GetBuffer());
			m_lstProperty.SetItemText(i, 1, ::theApp.PropertyDescription(code));
		}
	m_lstProperty.SortItemsEx(PropItemCompare, reinterpret_cast<LPARAM>(&m_lstProperty));
	//check items
	const int i = m_tabPropList.GetCurSel();
	ASSERT(0 <= i && i < int(size(m_aProps)));
	const auto & p = m_aProps[i];
	m_lstProperty.EnableWindow(p.second);
	//update desc and sort
	if (p.second) {
		for (auto & p : p.first.mProperty) {
			m_lstProperty.SetCheck(p.first, TRUE);
			m_lstProperty.SetItemText(p.first, 1, ::theApp.PropertyDescription(p.first, p.second));
		}
		m_lstProperty.SortItemsEx(PropItemCompare, reinterpret_cast<LPARAM>(&m_lstProperty));
	}
}

void CDlgFoundry::SetupPropParameters(int id, int value) {
	if (0 <= id) {
		ASSERT(0 <= value);
		const auto p = ::theApp.PropertyParameters(id, value);
		ASSERT(p.size() <= size(m_edParam));
		for (int i = 0; i < int(size(m_edParam)); ++i) {
			if (i < int(p.size())) {
				auto & a = p[i];
				m_edParam[i].SetWindowText(CSFormat(_T("%d"), a.iValue));
				m_sTip[i] = (CSFormat(_T("%d - %d"), a.iMin, a.iMax));
			} else {
				m_edParam[i].SetWindowText(_T(""));
				m_sTip[i] = (_T(""));
			}
			m_edParam[i].EnableWindow(i < int(p.size()));
		}
	} else
		for (int i = 0; i < int(size(m_edParam)); ++i) {
			m_edParam[i].SetWindowText(_T(""));
			m_edParam[i].EnableWindow(FALSE);
			m_sTip[i] = (_T(""));
		}
}

BOOL CDlgFoundry::ChangePropItemSelection(LPNMLISTVIEW pNMLV) {
	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED)) {
		//find out property list
		const int sel = m_tabPropList.GetCurSel();
		ASSERT(0 <= sel && sel < int(size(m_aProps)));
		const auto & p = m_aProps[sel].first.mProperty;
		//setup parameters
		m_iSelectedPropItem = -1;
		const auto code = _tstoi(m_lstProperty.GetItemText(pNMLV->iItem, 0).GetBuffer());
		const auto wh = p.find(code);
		if (wh == p.end())
			SetupPropParameters(-1, 0);
		else
			SetupPropParameters(wh->first, wh->second);
		m_iSelectedPropItem = pNMLV->iItem;
		return TRUE;
	}
	return FALSE;
}

BOOL CDlgFoundry::ChangePropItemChecked(LPNMLISTVIEW pNMLV) {
	BOOL bPrevState = (BOOL)(((pNMLV->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1);
	if (bPrevState < 0)
		bPrevState = 0;
	BOOL bChecked = (BOOL)(((pNMLV->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1);
	if (bChecked < 0)
		bChecked = 0;
	if (bPrevState == bChecked)
		return FALSE;
	//find out property list
	const int sel = m_tabPropList.GetCurSel();
	ASSERT(0 <= sel && sel < int(size(m_aProps)));
	auto & p = m_aProps[sel].first.mProperty;
	//handle item
	const int idx = pNMLV->iItem;
	const BOOL ret = (idx == m_iSelectedPropItem);
	const auto code = _tstoi(m_lstProperty.GetItemText(idx, 0).GetBuffer());
	if (bChecked) {	//check a new item
		auto wh = p.find(code);
		if (wh == p.end())
			p[code] = ::theApp.PropertyMetaData(code).DefaultValue();
		if (ret)
			SetupPropParameters(code, p[code]);
	} else {			//uncheck an item
		p.erase(code);
		m_lstProperty.SetItemText(idx, 1, ::theApp.PropertyDescription(code));
		if (ret)
			SetupPropParameters(-1, 0);
	}
	return ret;
}

void CDlgFoundry::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	BOOL update = ChangePropItemSelection(pNMLV);
	update = (ChangePropItemChecked(pNMLV) || update);
	if(update)
		UpdateData(FALSE);
	*pResult = 0;
}

void CDlgFoundry::OnEnChangeParam() {
	//prop item & meta
	if (m_iSelectedPropItem < 0)
		return;
	const auto code = _tstoi(m_lstProperty.GetItemText(m_iSelectedPropItem, 0).GetBuffer());
	auto & meta = ::theApp.PropertyMetaData(code);
	//params
	vector<int> p;
	for (auto & ed : m_edParam) {
		CString s;
		ed.GetWindowText(s);
		p.push_back(_tstoi(s.GetBuffer()));
	}
	const int v = meta.GetValue(p);
	if (v < 0)
		return;
	//update desc
	m_lstProperty.SetItemText(m_iSelectedPropItem, 1, ::theApp.PropertyDescription(code, v));
	//update prop list
	const int sel = m_tabPropList.GetCurSel();
	ASSERT(0 <= sel && sel < int(size(m_aProps)));
	auto & pp = m_aProps[sel].first.mProperty;
	pp[code] = v;
}

void CDlgFoundry::OnOK() {
	// TODO

	CDialogEx::OnOK();
}
