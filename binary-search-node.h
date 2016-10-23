#include<iostream>

using namespace std;
template<typename T> class has_equals{/*{{{*/
private:
    template<typename U>
    static auto check( U v ) -> decltype( v.equals(), std::true_type() );
    static auto check( ... ) -> decltype( std::false_type() );

public:
    typedef decltype( check( std::declval<T>() ) ) type;
    static bool const value = type::value;
};/*}}}*/

template<class T> class binarySearchNode {
public:
    binarySearchNode<T>* lch;
    binarySearchNode<T>* rch;
    T val;
    bool empty;
    binarySearchNode();
    binarySearchNode(T _val);
    void insert(const T& _val);
    void remove(const T& _val);
    void swap(const T& a, const T& b);
    void above_s(T _val, T& max);
    void below_s(T _val, T& min);
};
template<class T> binarySearchNode<T>::binarySearchNode() : lch(nullptr), rch(nullptr), val(), empty(true) {};
template<class T> binarySearchNode<T>::binarySearchNode(T _val) : lch(nullptr), rch(nullptr), val(_val), empty(false) {};
template<class T> void binarySearchNode<T>::insert(const T& _val) {/*{{{*/
    if (empty) {
        val = _val;
        empty = false;
    } else if (_val < val) {
        if (lch == nullptr) {
            lch = new binarySearchNode<T>(_val);
        } else {
            lch->insert(_val);
        }
    } else {
        if (rch == nullptr) {
            rch = new binarySearchNode<T>(_val);
        } else {
            rch->insert(_val);
        }
    }
}/*}}}*/
template<class T> void binarySearchNode<T>::remove(const T& _val) {/*{{{*/
    binarySearchNode<T>** _del = nullptr;
    binarySearchNode<T>* del = nullptr;

    if (_val < val) _del = &lch;
    else if (_val > val)  _del = &rch;
    else if (_val == val) {
        cout << "1" << endl;
        if (lch == nullptr && rch == nullptr) empty = true;
        else if (lch != nullptr && rch != nullptr) {
            binarySearchNode<T>* ch = lch;
            binarySearchNode<T>* parent = this;
            while (true) {
                if (ch->rch != nullptr) {
                    parent = ch;
                    ch = ch->rch;
                }
                else break;
            }
            val = ch->val;
            parent->rch = nullptr;
        } else if (lch != nullptr && rch == nullptr) {
            val = lch->val;
            rch = lch->rch;
            lch = lch->lch;
        } else if (lch != nullptr && rch != nullptr) {
            val = rch->val;
            lch = rch->lch;
            rch = rch->rch;
        }
    } else {
        del = *_del;
        if (del->val == _val) {
            cout << "2" << endl;
            if (del->lch == nullptr && del->rch == nullptr) *_del = nullptr;
            else if (del->lch != nullptr && del->rch != nullptr) {
                binarySearchNode<T>* ch = del->lch;
                binarySearchNode<T>* parent = del;
                while (true) {
                    if (ch->rch != nullptr) {
                        parent = ch;
                        ch = ch->rch;
                    }
                    else break;
                }
                val = ch->val;
                parent->rch = nullptr;
            } else if (del->lch != nullptr && del->rch == nullptr) *_del = del->lch;
            else if (del->lch != nullptr && del-> rch != nullptr) *_del = del->rch;
        } else {
            cout << "3" << endl;
            del->remove(_val);
        }
    }
}/*}}}*/
template<class T> void binarySearchNode<T>::swap(const T& a, const T& b) {/*{{{*/
    binarySearchNode<T>* _a = nullptr;
    binarySearchNode<T>* _b = nullptr;

    if (val > a) _a = lch;
    else if (val < a) _a = rch;

    if (val > b) _b = lch;
    else if (val < b) _b = rch;

    const bool T_has_equals = has_equals<T>::value;
    if (a == val && (!T_has_equals || a.equals(val))) {
        while (b != _b->val) {
            if (_b->val > b) _b = _b->lch;
            else _b = _b->rch;
        }
        T tmpVal = val;
        val = _b->val;
        _b->val = tmpVal;
        return;
    }
    if (b == val && (!T_has_equals || b.equals(val))) {
        while (a != _a->val) {
            if (_a->val > a) _a = _a->lch;
            else _a = _a->rch;
        }
        T tmpVal = val;
        val = _a->val;
        _a->val = tmpVal;
        return;
    }

    while (a != _a->val && (!T_has_equals || !a.equals(_a->val))) {
        if (_a->val > a) _a = _a->lch;
        else _a = _a->rch;
    }
    while (b != _b->val && (!T_has_equals || !b.equals(_b->val))) {
        if (_b->val > b) _b = _b->lch;
        else _b = _b->rch;
    }
    T tmpVal = _a->val;
    _a->val = _b->val;
    _b->val = tmpVal;
}/*}}}*/
int indent = 0;
template<class T> ostream& operator<<(ostream& os, const binarySearchNode<T>& bsn) {/*{{{*/
    os << "val: " << bsn.val;
    string indents = "";
    int i;
    for (i = 0; i < indent; ++i) indents += "  ";
    if (bsn.lch != nullptr && bsn.rch != nullptr) {
        ++indent;
        os << ", \n" + indents + "  L: " << *bsn.lch << ", \n" + indents + "  " << "R: " << *bsn.rch;
        --indent;
    } else if (bsn.lch == nullptr && bsn.rch != nullptr) {
        ++indent;
        os << ", \n" + indents + "  R: " << *bsn.rch;
        --indent;
    } else if (bsn.lch != nullptr && bsn.rch == nullptr) {
        ++indent;
        os << ", \n" + indents + "  L: " << *bsn.lch;
        --indent;
    }
    return os;
}/*}}}*/
template<class T> void binarySearchNode<T>::above_s(T _val, T& max) {/*{{{*/
    if (val > _val) {
        max = val;
        if (lch != nullptr) lch->above_s(_val, max);
    } else if (val <= _val && rch != nullptr) {
        rch->above_s(_val, max);
    }
}/*}}}*/
template<class T> void binarySearchNode<T>::below_s(T _val, T& min) {/*{{{*/
    if (val > _val && lch != nullptr) {
        lch->below_s(_val, min);
    } else if (val == _val && lch != nullptr) {
        lch->below_s(_val, min);
    } else if (val < _val) {
        min = val;
        if (rch != nullptr) rch->below_s(_val, min);
    }
}/*}}}*/
