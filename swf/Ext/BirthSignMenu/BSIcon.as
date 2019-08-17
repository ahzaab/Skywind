class BSIcon extends MovieClip
{
	/* PRIVATE VARIABLES */

	private var _id: Number;

	/* STAGE ELEMENTS */

	public var description: TextField;


	/* INITIALIZATION */

	public function BSIcon()
	{
		super();
	}


	/* PUBLIC FUNCTIONS */

	// @override MovieClip
	public function onRollOver(): Void
	{
		showDescription();
	}


	// @override MovieClip
	public function onRollOut(): Void
	{
		hideDescription();
	}


	// @override MovieClip
	public function onPress(): Void
	{
		_parent.changeMenu(_id);
	}


	public function showDescription(): Void
	{
		this.description._visible = true;
	}


	public function hideDescription(): Void
	{
		this.description._visible = false;
	}


	public function setDescription(a_name: String): Void
	{
		this.description.text = a_name;
	}


	public function setID(a_id: Number): Void
	{
		_id = a_id;
	}
}
